# Brewin interpreter v3 — adds typed variables (VarCell), closures, first-class functions,
# and block scoping. Functions are indexed by (name, type_signature). Each FuncContext
# maintains a stack of scope frames for variable resolution.

from intbase import InterpreterBase, ErrorType
from brewparse import parse_program

class ReturnSignal(Exception):
    def __init__(self, value):
        self.value = value

class VarCell:
    def __init__(self, vtype, value):
        self.vtype = vtype
        self.value = value

class FuncContext:
    def __init__(self, func_info):
        self.func_info = func_info
        self.frames = [ {} ]
        self.defined_names = set()

class Interpreter(InterpreterBase):
    def __init__(self, console_output=True, inp=None, trace_output=False):
        super().__init__(console_output, inp)
        self.trace_output = trace_output
        self.funcs = {}
        self.call_stack = []

    def run(self, program: str):
        ast = parse_program(program)
        self.index_funcs(ast)
        if ("main", "") not in self.funcs:
            self.error(ErrorType.NAME_ERROR, "No main() function found with correct signature")
            return
        main_info = self.funcs[("main", "")]
        try:
            self.call_user_function(main_info, [], [])
        except ReturnSignal:
            pass

    def index_funcs(self, program_node):
        for f in program_node.dict.get("functions", []):
            name = f.dict["name"]
            if name == "main":
                rettype = "v"
            else:
                if not name or name[-1] not in "isbov":
                    self.error(ErrorType.TYPE_ERROR, f"Function {name} has invalid return type suffix")
                    continue
                rettype = name[-1]
            args = f.dict.get("args", [])
            param_names = []
            param_types = []
            param_refs = []
            for a in args:
                pname = a.dict["name"]
                if not pname or pname[-1] not in "isbo":
                    self.error(ErrorType.TYPE_ERROR, f"Parameter {pname} has invalid type suffix")
                    continue
                ptype = pname[-1]
                pref = bool(a.dict.get("ref", False))
                param_names.append(pname)
                param_types.append(ptype)
                param_refs.append(pref)
            sig = "".join(param_types)
            key = (name, sig)
            if key in self.funcs:
                self.error(ErrorType.NAME_ERROR, f"Duplicate function definition for {name} with signature {sig}")
                continue
            self.funcs[key] = {
                "node": f,
                "name": name,
                "rettype": rettype,
                "param_names": param_names,
                "param_types": param_types,
                "param_refs": param_refs,
            }

    def current_context(self):
        return self.call_stack[-1] if self.call_stack else None

    def default_value_for_type(self, t):
        if t == "i":
            return 0
        if t == "s":
            return ""
        if t == "b":
            return False
        if t == "o":
            return self.nil()
        if t == "v":
            return None
        return None

    def get_value_type(self, v):
        if isinstance(v, bool):
            return "b"
        if isinstance(v, int):
            return "i"
        if isinstance(v, str):
            return "s"
        return "o"

    def nil(self):
        return None

    def to_brewin_string(self, v):
        if isinstance(v, bool):
            return "true" if v else "false"
        return "" if v is None else str(v)

    def lookup_var_cell(self, name, ctx=None):
        if ctx is None:
            ctx = self.current_context()
        if ctx is None:
            self.error(ErrorType.NAME_ERROR, f"Variable {name} not in any context")
            return None
        for frame in reversed(ctx.frames):
            if name in frame:
                return frame[name]
        self.error(ErrorType.NAME_ERROR, f"Variable {name} has not been defined")
        return None

    def declare_variable(self, name, block_scope):
        if not name or name[-1] not in "isbo":
            self.error(ErrorType.TYPE_ERROR, f"Variable {name} has invalid type suffix")
            return
        ctx = self.current_context()
        if ctx is None:
            self.error(ErrorType.NAME_ERROR, "No active function context")
            return
        if name in ctx.defined_names:
            self.error(ErrorType.NAME_ERROR, f"Variable {name} already defined in this function")
            return
        ctx.defined_names.add(name)
        vtype = name[-1]
        val = self.default_value_for_type(vtype)
        if block_scope:
            ctx.frames[-1][name] = VarCell(vtype, val)
        else:
            ctx.frames[0][name] = VarCell(vtype, val)

    def resolve_dotted_for_read(self, parts, ctx=None):
        base_name = parts[0]
        base_cell = self.lookup_var_cell(base_name, ctx)
        if base_cell is None:
            return None, None, None
        if base_cell.vtype != "o":
            self.error(ErrorType.TYPE_ERROR, "Base of dotted expression must be object-typed")
            return None, None, None
        obj = base_cell.value
        if obj is None:
            self.error(ErrorType.FAULT_ERROR, "Nil object dereference")
            return None, None, None
        cur_obj = obj
        for seg in parts[1:-1]:
            if not seg.endswith("o"):
                self.error(ErrorType.TYPE_ERROR, "Intermediate dotted segment must be object-typed")
                return None, None, None
            if not isinstance(cur_obj, dict):
                self.error(ErrorType.TYPE_ERROR, "Intermediate base is not an object")
                return None, None, None
            if seg not in cur_obj:
                self.error(ErrorType.NAME_ERROR, f"Field {seg} does not exist")
                return None, None, None
            cell = cur_obj[seg]
            if cell.vtype != "o":
                self.error(ErrorType.TYPE_ERROR, "Intermediate field is not object-typed")
                return None, None, None
            cur_obj = cell.value
            if cur_obj is None:
                self.error(ErrorType.FAULT_ERROR, "Nil object dereference")
                return None, None, None
        return base_cell, cur_obj, parts[-1]

    def get_qname_value(self, name_str):
        parts = name_str.split(".")
        if len(parts) == 1:
            cell = self.lookup_var_cell(parts[0])
            if cell is None:
                return self.nil()
            return cell.value
        base_cell, cur_obj, final_seg = self.resolve_dotted_for_read(parts)
        if cur_obj is None:
            return self.nil()
        if not isinstance(cur_obj, dict):
            self.error(ErrorType.TYPE_ERROR, "Final base is not an object")
            return self.nil()
        if final_seg not in cur_obj:
            self.error(ErrorType.NAME_ERROR, f"Field {final_seg} does not exist")
            return self.nil()
        cell = cur_obj[final_seg]
        return cell.value

    def get_qname_cell_for_ref(self, name_str, ctx=None):
        parts = name_str.split(".")
        if len(parts) == 1:
            return self.lookup_var_cell(parts[0], ctx)
        base_cell, cur_obj, final_seg = self.resolve_dotted_for_read(parts, ctx)
        if cur_obj is None:
            return None
        if not isinstance(cur_obj, dict):
            self.error(ErrorType.TYPE_ERROR, "Final base is not an object")
            return None
        if final_seg not in cur_obj:
            self.error(ErrorType.NAME_ERROR, f"Field {final_seg} does not exist")
            return None
        return cur_obj[final_seg]

    def assign_qname(self, name_str, value):
        vtype = self.get_value_type(value)
        parts = name_str.split(".")
        if len(parts) == 1:
            cell = self.lookup_var_cell(parts[0])
            if cell is None:
                return
            if cell.vtype != vtype:
                self.error(ErrorType.TYPE_ERROR, f"Type mismatch in assignment to {parts[0]}")
                return
            cell.value = value
            return
        base_cell, cur_obj, final_seg = self.resolve_dotted_for_read(parts)
        if cur_obj is None:
            return
        if not isinstance(cur_obj, dict):
            self.error(ErrorType.TYPE_ERROR, "Final base is not an object")
            return
        ftype = final_seg[-1] if final_seg else None
        if ftype not in "isbo":
            self.error(ErrorType.TYPE_ERROR, f"Field {final_seg} has invalid type suffix")
            return
        if ftype != vtype:
            self.error(ErrorType.TYPE_ERROR, "Type mismatch in field assignment")
            return
        if final_seg in cur_obj:
            cell = cur_obj[final_seg]
            cell.value = value
        else:
            cur_obj[final_seg] = VarCell(ftype, value)

    def exec_block(self, statements):
        if statements is None:
            return
        ctx = self.current_context()
        ctx.frames.append({})
        try:
            for s in statements:
                self.exec_stmt(s)
        finally:
            ctx.frames.pop()

    def exec_stmt(self, stmt):
        et = stmt.elem_type
        if et == "vardef":
            self.declare_variable(stmt.dict["name"], block_scope=False)
        elif et == "bvardef":
            self.declare_variable(stmt.dict["name"], block_scope=True)
        elif et == "=":
            rhs = self.eval_expr(stmt.dict["expression"])
            self.assign_qname(stmt.dict["var"], rhs)
        elif et == "fcall":
            fname = stmt.dict["name"]
            args = stmt.dict.get("args", [])
            if fname == "print":
                out = ""
                for a in args:
                    v = self.eval_expr(a)
                    out += self.to_brewin_string(v)
                self.output(out)
            elif fname == "inputi" or fname == "inputs":
                self.eval_fcall_expr(stmt)
            else:
                self.call_function(fname, args, from_expr=False)
        elif et == "if":
            cond = self.eval_expr(stmt.dict["condition"])
            if not isinstance(cond, bool):
                self.error(ErrorType.TYPE_ERROR, "If condition must be boolean")
                return
            if cond:
                self.exec_block(stmt.dict.get("statements"))
            else:
                self.exec_block(stmt.dict.get("else_statements"))
        elif et == "while":
            while True:
                cond = self.eval_expr(stmt.dict["condition"])
                if not isinstance(cond, bool):
                    self.error(ErrorType.TYPE_ERROR, "While condition must be boolean")
                    return
                if not cond:
                    break
                self.exec_block(stmt.dict.get("statements"))
        elif et == "return":
            expr = stmt.dict.get("expression")
            self.handle_return(expr)
        else:
            self.error(ErrorType.NAME_ERROR, f"Unknown statement type: {et}")

    def handle_return(self, expr):
        ctx = self.current_context()
        if ctx is None:
            self.error(ErrorType.NAME_ERROR, "Return outside of function")
            return
        fname = ctx.func_info["name"]
        rettype = ctx.func_info["rettype"]
        if fname == "main":
            if expr is not None:
                self.error(ErrorType.TYPE_ERROR, "main may not return a value")
                return
            raise ReturnSignal(None)
        if expr is None:
            value = self.default_value_for_type(rettype)
            raise ReturnSignal(value)
        value = self.eval_expr(expr)
        vtype = self.get_value_type(value)
        if rettype == "v":
            self.error(ErrorType.TYPE_ERROR, "Void function cannot return a value")
            return
        if vtype != rettype:
            self.error(ErrorType.TYPE_ERROR, "Return type mismatch")
            return
        raise ReturnSignal(value)

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
        if et == "@":
            return {}
        if et == "qname":
            return self.get_qname_value(node.dict["name"])
        if et == "fcall":
            return self.eval_fcall_expr(node)
        if et == "neg":
            v = self.eval_expr(node.dict["op1"])
            if not isinstance(v, int) or isinstance(v, bool):
                self.error(ErrorType.TYPE_ERROR, "Unary - requires integer")
                return self.nil()
            return -v
        if et == "!":
            v = self.eval_expr(node.dict["op1"])
            if not isinstance(v, bool):
                self.error(ErrorType.TYPE_ERROR, "Logical ! requires boolean")
                return self.nil()
            return not v
        if et == "convert":
            return self.eval_convert(node)
        if et in ("+", "-", "*", "/", "==", "!=", "<", "<=", ">", ">=", "&&", "||"):
            left = self.eval_expr(node.dict["op1"])
            right = self.eval_expr(node.dict["op2"])
            if et in ("&&", "||"):
                if not (isinstance(left, bool) and isinstance(right, bool)):
                    self.error(ErrorType.TYPE_ERROR, "Logical operators require booleans")
                    return self.nil()
                return left and right if et == "&&" else left or right
            if et in ("==", "!="):
                lt = self.get_value_type(left)
                rt = self.get_value_type(right)
                if lt != rt:
                    eq = False
                elif lt == "o":
                    eq = (left is right)
                else:
                    eq = (left == right)
                return eq if et == "==" else (not eq)
            if et == "+" and isinstance(left, str) and isinstance(right, str):
                return left + right
            if et in ("+", "-", "*", "/"):
                if not (isinstance(left, int) and not isinstance(left, bool) and isinstance(right, int) and not isinstance(right, bool)):
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
                if not (isinstance(left, int) and not isinstance(left, bool) and isinstance(right, int) and not isinstance(right, bool)):
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

    def eval_convert(self, node):
        to_type = node.dict["to_type"]
        val = self.eval_expr(node.dict["expr"])
        vtype = self.get_value_type(val)
        if to_type == "int":
            if vtype == "i":
                return val
            if vtype == "s":
                s = val
                try:
                    return int(s)
                except ValueError:
                    self.error(ErrorType.TYPE_ERROR, "Invalid string to int conversion")
                    return self.nil()
            if vtype == "b":
                return 1 if val else 0
            self.error(ErrorType.TYPE_ERROR, "Cannot convert object to int")
            return self.nil()
        if to_type == "str":
            if vtype == "s":
                return val
            if vtype == "i":
                return str(val)
            if vtype == "b":
                return "true" if val else "false"
            self.error(ErrorType.TYPE_ERROR, "Cannot convert object to string")
            return self.nil()
        if to_type == "bool":
            if vtype == "b":
                return val
            if vtype == "i":
                return False if val == 0 else True
            if vtype == "s":
                return False if val == "" else True
            self.error(ErrorType.TYPE_ERROR, "Cannot convert object to bool")
            return self.nil()
        self.error(ErrorType.TYPE_ERROR, "Unknown conversion type")
        return self.nil()

    def eval_fcall_expr(self, node):
        fname = node.dict["name"]
        args = node.dict.get("args", [])
        if fname == "print":
            self.error(ErrorType.TYPE_ERROR, "print returns void and cannot be used in expressions")
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
        return self.call_function(fname, args, from_expr=True)

    def call_function(self, fname, arg_nodes, from_expr):
        arg_values = [self.eval_expr(a) for a in arg_nodes]
        arg_types = [self.get_value_type(v) for v in arg_values]
        sig = "".join(arg_types)
        key = (fname, sig)
        info = self.funcs.get(key)
        if info is None:
            self.error(ErrorType.NAME_ERROR, f"No matching function for {fname} with signature {sig}")
            return self.nil()
        if from_expr and info["rettype"] == "v":
            self.error(ErrorType.TYPE_ERROR, "Cannot use void function in expression")
            return self.nil()
        return self.call_user_function(info, arg_values, arg_nodes)

    def call_user_function(self, func_info, arg_values, arg_nodes):
        caller_ctx = self.current_context()
        ctx = FuncContext(func_info)
        for pname in func_info["param_names"]:
            ctx.defined_names.add(pname)
        self.call_stack.append(ctx)
        try:
            for i, pname in enumerate(func_info["param_names"]):
                ptype = func_info["param_types"][i]
                pref = func_info["param_refs"][i]
                if pref:
                    arg_node = arg_nodes[i]
                    if arg_node.elem_type != "qname":
                        self.error(ErrorType.TYPE_ERROR, "Reference parameter requires variable/field argument")
                        continue
                    cell = self.get_qname_cell_for_ref(arg_node.dict["name"], caller_ctx)
                    if cell is None:
                        continue
                    if cell.vtype != ptype:
                        self.error(ErrorType.TYPE_ERROR, "Reference parameter type mismatch")
                        continue
                    ctx.frames[0][pname] = cell
                else:
                    val = arg_values[i]
                    vtype = self.get_value_type(val)
                    if vtype != ptype:
                        self.error(ErrorType.TYPE_ERROR, "Parameter type mismatch")
                        continue
                    ctx.frames[0][pname] = VarCell(ptype, val)
            stmts = func_info["node"].dict.get("statements", [])
            self.exec_block(stmts)
            fname = func_info["name"]
            rettype = func_info["rettype"]
            if fname == "main":
                return None
            return self.default_value_for_type(rettype)
        except ReturnSignal as r:
            return r.value
        finally:
            self.call_stack.pop()
