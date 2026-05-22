# Brewin interpreter v2 — adds user-defined functions, a call stack, and return values.
# Functions are indexed by (name, arity). Each call pushes a fresh env dict; ReturnSignal
# unwinds the stack on return.

from intbase import InterpreterBase, ErrorType
from brewparse import parse_program

class ReturnSignal(Exception):
    def __init__(self, value):
        self.value = value

class Interpreter(InterpreterBase):
    def __init__(self, console_output=True, inp=None, trace_output=False):
        super().__init__(console_output, inp)
        self.trace_output = trace_output
        self.funcs = {}
        self.env = None

    def run(self, program: str):
        ast = parse_program(program)
        self.index_funcs(ast)
        if ("main", 0) not in self.funcs:
            self.error(ErrorType.NAME_ERROR, "No main() function found with arity 0")
            return
        try:
            self.call_user_func("main", [])
        except ReturnSignal:
            pass

    def index_funcs(self, program_node):
        for f in program_node.dict.get("functions", []):
            name = f.dict["name"]
            arity = len(f.dict.get("args", []))
            self.funcs[(name, arity)] = f

    def resolve_func(self, name, arity):
        node = self.funcs.get((name, arity))
        if node is None:
            self.error(ErrorType.NAME_ERROR, f"Function {name}({arity}) was not found")
        return node

    def call_user_func(self, name, arg_values):
        fn = self.resolve_func(name, len(arg_values))
        prev_env = self.env
        env = {}
        for i, arg_node in enumerate(fn.dict.get("args", [])):
            env[arg_node.dict["name"]] = arg_values[i]
        self.env = env
        try:
            for stmt in fn.dict.get("statements", []):
                self.exec_stmt(stmt)
        except ReturnSignal as r:
            self.env = prev_env
            return r.value
        self.env = prev_env
        return self.nil()

    def exec_stmt(self, stmt):
        et = stmt.elem_type
        if et == "vardef":
            name = stmt.dict["name"]
            if name in self.env:
                self.error(ErrorType.NAME_ERROR, f"Variable {name} already defined in this function")
                return
            self.env[name] = self.undefined_marker()
        elif et == "=":
            var = stmt.dict["var"]
            if var not in self.env:
                self.error(ErrorType.NAME_ERROR, f"Variable {var} has not been defined")
                return
            val = self.eval_expr(stmt.dict["expression"])
            self.env[var] = val
        elif et == "fcall":
            fname = stmt.dict["name"]
            args = stmt.dict.get("args", [])
            if fname == "print":
                out = "".join(self.to_brewin_string(self.eval_expr(a)) for a in args)
                self.output(out)
            else:
                self.eval_fcall_expr(stmt)
        elif et == "if":
            cond = self.eval_expr(stmt.dict["condition"])
            if not isinstance(cond, bool):
                self.error(ErrorType.TYPE_ERROR, "If condition must be boolean")
                return
            block = "statements" if cond else "else_statements"
            stmts = stmt.dict.get(block)
            if stmts:
                for s in stmts:
                    self.exec_stmt(s)
        elif et == "while":
            while True:
                cond = self.eval_expr(stmt.dict["condition"])
                if not isinstance(cond, bool):
                    self.error(ErrorType.TYPE_ERROR, "While condition must be boolean")
                    return
                if not cond:
                    break
                for s in stmt.dict.get("statements", []):
                    self.exec_stmt(s)
        elif et == "return":
            expr = stmt.dict.get("expression")
            value = self.nil() if expr is None else self.eval_expr(expr)
            raise ReturnSignal(value)
        else:
            self.error(ErrorType.NAME_ERROR, f"Unknown statement type: {et}")

    def eval_expr(self, node):
        et = node.elem_type
        if et == "int":
            return node.dict["val"]
        if et == "string":
            return node.dict["val"]
        if et == "bool":
            return node.dict["val"]
        if et == "nil":
            return self.nil()
        if et == "qname":
            name = node.dict["name"]
            if name not in self.env:
                self.error(ErrorType.NAME_ERROR, f"Variable {name} has not been defined")
                return self.nil()
            return self.env[name]
        if et == "fcall":
            return self.eval_fcall_expr(node)
        if et == "neg":
            v = self.eval_expr(node.dict["op1"])
            if type(v) is not int:
                self.error(ErrorType.TYPE_ERROR, "Unary - requires integer")
                return self.nil()
            return -v
        if et == "!":
            v = self.eval_expr(node.dict["op1"])
            if not isinstance(v, bool):
                self.error(ErrorType.TYPE_ERROR, "Logical ! requires boolean")
                return self.nil()
            return not v
        if et in ("+", "-", "*", "/", "==", "!=", "<", "<=", ">", ">=", "&&", "||"):
            left = self.eval_expr(node.dict["op1"])
            right = self.eval_expr(node.dict["op2"])
            if et in ("&&", "||"):
                if not (isinstance(left, bool) and isinstance(right, bool)):
                    self.error(ErrorType.TYPE_ERROR, "Logical operators require booleans")
                    return self.nil()
                return (left and right) if et == "&&" else (left or right)
            if et in ("==", "!="):
                if left is None and right is None:
                    eq = True
                elif left is None or right is None or type(left) is not type(right):
                    eq = False
                else:
                    eq = (left == right)
                return eq if et == "==" else (not eq)
            if et == "+" and isinstance(left, str) and isinstance(right, str):
                return left + right
            if et in ("+", "-", "*", "/"):
                if not (type(left) is int and type(right) is int):
                    self.error(ErrorType.TYPE_ERROR, "Arithmetic requires integers (except string +)")
                    return self.nil()
                if et == "/":
                    return left // right
                if et == "+":
                    return left + right
                if et == "-":
                    return left - right
                return left * right
            if et in ("<", "<=", ">", ">="):
                if not (type(left) is int and type(right) is int):
                    self.error(ErrorType.TYPE_ERROR, "Relational operators require integers")
                    return self.nil()
                if et == "<":
                    return left < right
                if et == "<=":
                    return left <= right
                if et == ">":
                    return left > right
                return left >= right
        self.error(ErrorType.NAME_ERROR, f"Unknown expression type: {et}")
        return self.nil()

    def eval_fcall_expr(self, node):
        fname = node.dict["name"]
        args = node.dict.get("args", [])
        if fname == "print":
            out = "".join(self.to_brewin_string(self.eval_expr(a)) for a in args)
            self.output(out)
            return self.nil()
        if fname == "inputi":
            if len(args) > 1:
                self.error(ErrorType.NAME_ERROR, "inputi: wrong number of arguments")
                return self.nil()
            if len(args) == 1:
                self.output(self.to_brewin_string(self.eval_expr(args[0])))
            s = self.get_input()
            try:
                return int(s)
            except ValueError:
                self.error(ErrorType.TYPE_ERROR, "inputi received non-integer")
                return self.nil()
        if fname == "inputs":
            if len(args) > 1:
                self.error(ErrorType.NAME_ERROR, "inputs: wrong number of arguments")
                return self.nil()
            if len(args) == 1:
                self.output(self.to_brewin_string(self.eval_expr(args[0])))
            return self.get_input()
        argv = [self.eval_expr(a) for a in args]
        return self.call_user_func(fname, argv)

    def to_brewin_string(self, v):
        if isinstance(v, bool):
            return "true" if v else "false"
        return "" if v is None else str(v)

    def nil(self):
        return None

    def undefined_marker(self):
        return None
