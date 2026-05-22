# Brewin interpreter v1 — variables, arithmetic (+/-), print, and inputi.
# Variables live in a single global scope dict (self.vars). No functions or control flow.

from intbase import InterpreterBase, ErrorType
from brewparse import parse_program

class Interpreter(InterpreterBase):
    def __init__(self, console_output=True, inp=None, trace_output=False):
        super().__init__(console_output, inp)
        self.trace_output = trace_output
        self.vars = {}

    def run(self, program: str):
        ast = parse_program(program)
        main_func = self.find_main(ast)
        if main_func is None:
            self.error(ErrorType.NAME_ERROR, "No main() function was found")
            return
        self.run_function(main_func)


    def find_main(self, program_node):
        for f in program_node.dict.get('functions', []):
            if f.dict.get('name') == 'main':
                return f
        return None
    
    def run_function(self, func_node):
        for stmt in func_node.dict.get('statements', []):
            if self.trace_output:
                print("TRACE stmt:", stmt.elem_type, stmt.dict)
            self.run_statement(stmt)

    def run_statement(self, stmt):
        et = stmt.elem_type
        if et == 'vardef':
            self._do_vardef(stmt)
        elif et == '=':
            self._do_assignment(stmt)
        elif et == 'fcall':
            self._do_fcall_stmt(stmt)
        else:

            self.error(ErrorType.NAME_ERROR, f"Unknown statement type: {et}")

    def _do_vardef(self, stmt):
        name = stmt.dict['name']
        if name in self.vars:
            self.error(ErrorType.NAME_ERROR, f"Variable {name} defined more than once")
            return
        self.vars[name] = None

    def _do_assignment(self,stmt):
        name = stmt.dict['var']
        if name not in self.vars:
            self.error(ErrorType.NAME_ERROR, f"Variable {name} has not been defined")
            return
        value = self.eval_expr(stmt.dict['expression'])
        self.vars[name] = value

    def _do_fcall_stmt(self, stmt):
        fname = stmt.dict['name']
        args = stmt.dict.get('args', [])
        if fname == 'print':
            out = ''.join(self.to_string(self.eval_expr(a)) for a in args)
            self.output(out)
        elif fname == 'inputi':
            _ = self.eval_inputi(args)
        else:
            self.error(ErrorType.NAME_ERROR, f"Function {fname} has not been defined")
    
    def eval_expr(self, node):
        et = node.elem_type
        if et == 'int':
            return node.dict['val']
        if et == 'string':
            return node.dict['val']
        if et == 'qname':
            name = node.dict['name']
            if name not in self.vars:
                self.error(ErrorType.NAME_ERROR, f"Variable {name} has not been defined")
                return None
            return self.vars[name]
        if et in ('+', '-'):
            left = self.eval_expr(node.dict['op1'])
            right = self.eval_expr(node.dict['op2'])
            if not isinstance(left, int) or not isinstance(right, int):
                self.error(ErrorType.TYPE_ERROR, "Incompatible types for arithmetic operation")
                return None
            return left + right if et == '+' else left - right
        if et == 'fcall':
            fname = node.dict['name']
            args = node.dict.get('args', [])
            if fname == 'inputi':
                return self.eval_inputi(args)
            elif fname == 'print':
                out = ''.join(self.to_string(self.eval_expr(a)) for a in args)
                self.output(out)
                return None
            else:
                self.error(ErrorType.NAME_ERROR, f"Function {fname} has not been defined")
                return None
        self.error(ErrorType.NAME_ERROR, f"Unknown expression type: {et}")
        return None
    
    def eval_inputi(self, args):
        if len(args) > 1:
            self.error(ErrorType.NAME_ERROR, "No inputi() function found that takes > 1 parameter")
            return None
        if len(args) == 1:
            prompt_val = self.eval_expr(args[0])
            self.output(self.to_string(prompt_val))
        s = self.get_input()
        try:
            return int(s)
        except ValueError:
            return None
        
    def to_string(self, v):
        return '' if v is None else str(v)

