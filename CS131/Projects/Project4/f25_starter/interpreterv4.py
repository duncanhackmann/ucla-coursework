# Brewin interpreter v4 — full type system with Value wrappers, interface types,
# and object-oriented features. Type is inferred from variable name suffix (e.g. 'i'=int,
# uppercase letter = object implementing that interface).

from intbase import InterpreterBase, ErrorType
from brewparse import parse_program
from element import Element
from copy import copy
import enum

class Type(enum.Enum):
    INT = 1
    STRING = 2
    BOOL = 3
    OBJECT = 4
    FUNCTION = 5
    VOID = 6
    ERROR = 7

    @staticmethod
    def get_type(var_name):
        if not var_name:
            return Type.ERROR
        last_letter = var_name[-1]
        if last_letter == "i":
            return Type.INT
        if last_letter == "s":
            return Type.STRING
        if last_letter == "b":
            return Type.BOOL
        if last_letter == "o":
            return Type.OBJECT
        if last_letter == "f":
            return Type.FUNCTION
        if last_letter == "v":
            return Type.VOID
        if last_letter.isupper():
            return Type.OBJECT
        return Type.ERROR

def get_decl_type_and_iface(name):
    last = name[-1]
    if last in "isbofv":
        t = Type.get_type(name)
        iface = None
    elif last.isupper():
        t = Type.OBJECT
        iface = last
    else:
        t = Type.ERROR
        iface = None
    return t, iface

class Value:
    def __init__(self, t, v=None):
        if v is None:
            self.t = t
            self.v = self.__default_value_for_type(t)
        else:
            self.t = t
            self.v = v

    def set(self, other):
        self.t = other.t
        self.v = other.v

    def __default_value_for_type(self, t):
        if t == Type.INT:
            return 0
        if t == Type.STRING:
            return ""
        if t == Type.BOOL:
            return False
        if t == Type.OBJECT:
            return None
        if t == Type.FUNCTION:
            return None
        if t == Type.VOID:
            return None
        raise Exception("invalid default value for type")

class Environment:
    def __init__(self):
        self.env = []

    def enter_block(self):
        self.env[-1].append({})

    def exit_block(self):
        self.env[-1].pop()

    def enter_func(self):
        self.env.append([{}])

    def exit_func(self):
        self.env.pop()

    def fdef(self, varname, value):
        top_env = self.env[-1]
        func_block = top_env[0]
        if varname in func_block:
            return False
        func_block[varname] = value
        return True

    def bdef(self, varname, value):
        top_env = self.env[-1]
        curr_block = top_env[-1]
        if varname in curr_block:
            return False
        curr_block[varname] = value
        return True

    def exists(self, varname):
        top_env = self.env[-1]
        for block in reversed(top_env):
            if varname in block:
                return True
        return False

    def get(self, varname):
        top_env = self.env[-1]
        for block in reversed(top_env):
            if varname in block:
                return block[varname]
        return None

    def set(self, varname, value):
        top_env = self.env[-1]
        for block in reversed(top_env):
            if varname in block:
                block[varname] = value
                return True
        return False

class ParamSpec:
    def __init__(self, name, t, iface, is_ref):
        self.name = name
        self.t = t
        self.iface = iface
        self.is_ref = is_ref

class FuncDef:
    def __init__(self, func_ast, is_lambda=False):
        self.ast = func_ast
        self.name = func_ast.get("name")
        self.is_lambda = is_lambda
        self.params = []
        for a in func_ast.get("args"):
            pname = a.get("name")
            ptype, piface = get_decl_type_and_iface(pname)
            self.params.append(ParamSpec(pname, ptype, piface, a.get("ref")))
        self.return_type, self.return_iface = self.__get_return_type(func_ast)
        self.statements = func_ast.get("statements")
        self.param_type_sig = self.__compute_param_sig()

    def __get_return_type(self, func_ast):
        name = func_ast.get("name")
        if name == "main":
            return Type.VOID, None
        last = name[-1]
        if last == "v":
            return Type.VOID, None
        if last == "f":
            return Type.FUNCTION, None
        if last.isupper():
            return Type.OBJECT, last
        t = Type.get_type(name)
        return t, None

    def __compute_param_sig(self):
        sig = ""
        for p in self.params:
            if p.t == Type.INT:
                sig += "i"
            elif p.t == Type.STRING:
                sig += "s"
            elif p.t == Type.BOOL:
                sig += "b"
            elif p.t == Type.OBJECT:
                sig += "o"
            elif p.t == Type.FUNCTION:
                sig += "f"
            else:
                sig += "?"
        return sig

class FuncValue:
    def __init__(self, func_def, closure=None):
        self.func_def = func_def
        self.closure = closure or {}

class InterfaceSpec:
    def __init__(self, name):
        self.name = name
        self.var_fields = {}
        self.func_fields = {}

class CallContext:
    def __init__(self, closure=None):
        self.closure = closure or {}

class Interpreter(InterpreterBase):
    def __init__(self, console_output=True, inp=None, trace_output=False):
        super().__init__(console_output, inp)
        self.env = Environment()
        self.bops = {"+", "-", "*", "/", "==", "!=", ">", ">=", "<", "<=", "||", "&&"}
        self.funcs = {}
        self.funcs_by_name = {}
        self.named_func_values = {}
        self.interfaces = {}
        self.callstack = []

    def run(self, program):
        ast = parse_program(program)
        self.__process_interfaces(ast)
        self.__create_function_table(ast)
        call_element = Element(InterpreterBase.FCALL_NODE, name="main", args=[])
        self.__run_fcall(call_element)

    def __process_interfaces(self, ast):
        self.interfaces = {}
        iface_nodes = ast.get("interfaces") or []
        for iface_node in iface_nodes:
            name = iface_node.get("name")
            if not name or len(name) != 1 or not name.isupper():
                super().error(ErrorType.NAME_ERROR, "invalid interface name")
            if name in self.interfaces:
                super().error(ErrorType.NAME_ERROR, "interface redeclared")
            spec = InterfaceSpec(name)
            for field in iface_node.get("fields"):
                kind = field.elem_type
                fname = field.get("name")
                if fname in spec.var_fields or fname in spec.func_fields:
                    super().error(ErrorType.NAME_ERROR, "duplicate field in interface")
                if kind == "field_var":
                    t, iface = get_decl_type_and_iface(fname)
                    if t == Type.ERROR or t == Type.VOID:
                        super().error(ErrorType.TYPE_ERROR, "invalid field type in interface")
                    if iface and iface not in self.interfaces:
                        super().error(ErrorType.NAME_ERROR, "unknown interface in field")
                    spec.var_fields[fname] = (t, iface)
                elif kind == "field_func":
                    params = []
                    for arg in field.get("params"):
                        pname = arg.get("name")
                        pt, piface = get_decl_type_and_iface(pname)
                        if pt == Type.VOID or pt == Type.ERROR:
                            super().error(ErrorType.TYPE_ERROR, "invalid param type in interface func field")
                        if piface and piface not in self.interfaces:
                            super().error(ErrorType.NAME_ERROR, "unknown interface in func field")
                        params.append(ParamSpec(pname, pt, piface, arg.get("ref")))
                    spec.func_fields[fname] = params
                else:
                    super().error(ErrorType.TYPE_ERROR, "unknown interface field kind")
            self.interfaces[name] = spec

    def __get_parameters_type_signature(self, formal_params):
        sig = ""
        for p in formal_params:
            name = p.get("name")
            last = name[-1]
            if last in "isbof":
                sig += last
            elif last.isupper():
                sig += "o"
            else:
                super().error(ErrorType.TYPE_ERROR, "invalid type in formal parameter")
        return sig

    def __get_arguments_type_signature(self, actual_args):
        sig = ""
        for arg in actual_args:
            if arg.t == Type.INT:
                sig += "i"
            elif arg.t == Type.STRING:
                sig += "s"
            elif arg.t == Type.BOOL:
                sig += "b"
            elif arg.t == Type.OBJECT:
                sig += "o"
            elif arg.t == Type.FUNCTION:
                sig += "f"
            elif arg.t == Type.VOID:
                super().error(ErrorType.TYPE_ERROR, "void type not allowed as parameter")
            else:
                super().error(ErrorType.TYPE_ERROR, "invalid param type")
        return sig

    def __create_function_table(self, ast):
        self.funcs = {}
        self.funcs_by_name = {}
        self.named_func_values = {}
        functions = ast.get("functions") or []
        for func in functions:
            name = func.get("name")
            if name != "main":
                last = name[-1]
                if not (last in "isbofv" or last.isupper()):
                    super().error(ErrorType.TYPE_ERROR, "invalid function return type")
            param_type_sig = self.__get_parameters_type_signature(func.get("args"))
            fdef = FuncDef(func, is_lambda=False)
            if fdef.return_type == Type.ERROR:
                super().error(ErrorType.TYPE_ERROR)
            type_sig = (name, param_type_sig)
            if type_sig in self.funcs:
                super().error(ErrorType.NAME_ERROR, "function already defined")
            if name not in self.funcs_by_name:
                self.funcs_by_name[name] = []
            for existing in self.funcs_by_name[name]:
                if existing.param_type_sig == fdef.param_type_sig:
                    super().error(ErrorType.NAME_ERROR, "duplicate overloaded function")
            self.funcs[type_sig] = fdef
            self.funcs_by_name[name].append(fdef)
            if name not in self.named_func_values:
                self.named_func_values[name] = FuncValue(fdef)

    def __get_function(self, name, param_type_signature=""):
        if (name, param_type_signature) in self.funcs:
            return self.funcs[(name, param_type_signature)]
        if name not in self.funcs_by_name:
            super().error(ErrorType.NAME_ERROR, "function not found")
        super().error(ErrorType.TYPE_ERROR, "no matching overload for call")

    def __run_vardef(self, statement, block_def=False):
        name = statement.get("name")
        t, iface = get_decl_type_and_iface(name)
        if t == Type.ERROR or t == Type.VOID:
            super().error(ErrorType.TYPE_ERROR, "invalid variable type")
        default_value = Value(t)
        if block_def:
            if not self.env.bdef(name, default_value):
                super().error(ErrorType.NAME_ERROR, "variable already defined in block")
        else:
            if not self.env.fdef(name, default_value):
                super().error(ErrorType.NAME_ERROR, "variable already defined at function scope")

    def __assign_simple_var(self, name, rvalue):
        if self.env.env and self.env.exists(name):
            cell = self.env.get(name)
        elif self.callstack and name in self.callstack[-1].closure:
            cell = self.callstack[-1].closure[name]
        else:
            super().error(ErrorType.NAME_ERROR, "variable not defined")
        dest_t, dest_iface = get_decl_type_and_iface(name)
        if dest_t == Type.ERROR or dest_t == Type.VOID:
            super().error(ErrorType.TYPE_ERROR, "invalid assignment target")
        new_val = self.__coerce_assignment_type(dest_t, dest_iface, rvalue)
        cell.set(new_val)

    def __coerce_assignment_type(self, dest_t, dest_iface, rvalue):
        is_nil = (rvalue.t == Type.OBJECT and rvalue.v is None) or (rvalue.t == Type.FUNCTION and rvalue.v is None)
        if dest_t in (Type.OBJECT, Type.FUNCTION):
            if is_nil:
                return Value(dest_t, None)
        if rvalue.t != dest_t:
            super().error(ErrorType.TYPE_ERROR, "type mismatch in assignment")
        if dest_t == Type.OBJECT and dest_iface:
            self.__check_interface_satisfied(rvalue, dest_iface)
        return rvalue

    def __run_assign(self, statement):
        name = statement.get("var")
        dotted = name.split(".")
        rvalue = self.eval_expr(statement.get("expression"))
        if len(dotted) == 1:
            self.__assign_simple_var(name, rvalue)
            return
        base_name = dotted[0]
        if self.callstack and base_name in self.callstack[-1].closure:
            lvalue = self.callstack[-1].closure[base_name]
        elif self.env.env and self.env.exists(base_name):
            lvalue = self.env.get(base_name)
        else:
            super().error(ErrorType.NAME_ERROR, "variable not defined")
        if lvalue.t != Type.OBJECT:
            super().error(ErrorType.TYPE_ERROR, "cannot access member of non-object")
        if lvalue.v is None:
            super().error(ErrorType.FAULT_ERROR, "cannot dereference nil object")
        suffix = dotted[1:-1]
        for i, sub in enumerate(suffix):
            if lvalue.v is None:
                super().error(ErrorType.FAULT_ERROR, "cannot dereference nil member object")
            if sub not in lvalue.v:
                super().error(ErrorType.NAME_ERROR, "object member not found")
            if i < len(suffix) - 1 and not (sub[-1] == "o" or sub[-1].isupper()):
                super().error(ErrorType.TYPE_ERROR, "intermediate member must be object/interface")
            inner = lvalue.v[sub]
            if inner.t != Type.OBJECT:
                super().error(ErrorType.TYPE_ERROR, "intermediate member must be object")
            if inner.v is None:
                super().error(ErrorType.FAULT_ERROR, "cannot dereference nil member object")
            lvalue = inner
        final_name = dotted[-1]
        dest_t, dest_iface = get_decl_type_and_iface(final_name)
        if dest_t == Type.ERROR or dest_t == Type.VOID:
            super().error(ErrorType.TYPE_ERROR, "invalid member type")
        new_val = self.__coerce_assignment_type(dest_t, dest_iface, rvalue)
        if new_val.t in (Type.OBJECT, Type.FUNCTION):
            lvalue.v[final_name] = new_val
        else:
            lvalue.v[final_name] = Value(new_val.t, new_val.v)

    def __handle_input(self, fcall_name, args):
        if len(args) > 1:
            super().error(ErrorType.NAME_ERROR, "too many arguments for input function")
        if args:
            self.__handle_print(args)
        res = super().get_input()
        if fcall_name == "inputi":
            return Value(Type.INT, int(res))
        return Value(Type.STRING, res)

    def __handle_print(self, args):
        out = ""
        for arg in args:
            c_out = self.eval_expr(arg)
            if c_out.t == Type.VOID:
                super().error(ErrorType.TYPE_ERROR, "cannot pass void argument to function")
            if c_out.t == Type.BOOL:
                out += str(c_out.v).lower()
            else:
                out += str(c_out.v)
        super().output(out)
        return Value(Type.VOID, None)

    def __run_fcall(self, func_call_ast):
        fcall_name, args = func_call_ast.get("name"), func_call_ast.get("args")
        if fcall_name in ("inputi", "inputs"):
            return self.__handle_input(fcall_name, args)
        if fcall_name == "print":
            return self.__handle_print(args)
        if "." in fcall_name:
            return self.__run_method_call(fcall_name, args)
        if self.env.env and self.env.exists(fcall_name):
            var_val = self.env.get(fcall_name)
        elif self.callstack and fcall_name in self.callstack[-1].closure:
            var_val = self.callstack[-1].closure[fcall_name]
        else:
            var_val = None
        if var_val is not None and var_val.t == Type.FUNCTION:
            return self.__call_function_value(var_val, args, self_obj=None)
        actual_args = [self.eval_expr(a) for a in args]
        args_type_sig = self.__get_arguments_type_signature(actual_args)
        func_def = self.__get_function(fcall_name, args_type_sig)
        return self.__call_named_function(func_def, actual_args)

    def __call_named_function(self, func_def, actual_args):
        self.callstack.append(CallContext())
        self.env.enter_func()
        if len(actual_args) != len(func_def.params):
            super().error(ErrorType.TYPE_ERROR, "wrong arg count")
        for param, actual in zip(func_def.params, actual_args):
            self.__check_arg_type_compat(actual, param)
            bound = self.__clone_for_passing(actual, param.is_ref)
            if not self.env.fdef(param.name, bound):
                super().error(ErrorType.NAME_ERROR, "parameter name conflict")
        res, _ = self.__run_statements(func_def, func_def.statements)
        self.env.exit_func()
        self.callstack.pop()
        return res

    def __run_method_call(self, qname, arg_nodes):
        parts = qname.split(".")
        base_name = parts[0]
        if self.env.env and self.env.exists(base_name):
            obj_val = self.env.get(base_name)
        elif self.callstack and base_name in self.callstack[-1].closure:
            obj_val = self.callstack[-1].closure[base_name]
        else:
            super().error(ErrorType.NAME_ERROR, "base object not defined")
        if obj_val.t != Type.OBJECT:
            super().error(ErrorType.TYPE_ERROR, "base is not object")
        if obj_val.v is None:
            super().error(ErrorType.FAULT_ERROR, "method call on nil")
        for sub in parts[1:-1]:
            if obj_val.v is None:
                super().error(ErrorType.FAULT_ERROR, "nil reference access")
            if sub not in obj_val.v:
                super().error(ErrorType.NAME_ERROR, "object member not found")
            mid = obj_val.v[sub]
            if mid.t != Type.OBJECT:
                super().error(ErrorType.TYPE_ERROR, "member must be object")
            obj_val = mid
        last = parts[-1]
        if obj_val.v is None:
            super().error(ErrorType.FAULT_ERROR, "nil reference access")
        if last not in obj_val.v:
            super().error(ErrorType.NAME_ERROR, "method field not found")
        fval = obj_val.v[last]
        if fval.t != Type.FUNCTION:
            super().error(ErrorType.TYPE_ERROR, "member is not function")
        return self.__call_function_value(fval, arg_nodes, self_obj=obj_val)

    def __clone_for_passing(self, arg, ref_param):
        if ref_param:
            return arg
        return copy(arg)

    def __check_arg_type_compat(self, actual, param):
        is_nil = (actual.t == Type.OBJECT and actual.v is None) or (actual.t == Type.FUNCTION and actual.v is None)
        if is_nil:
            if param.t in (Type.OBJECT, Type.FUNCTION):
                return
        if actual.t != param.t:
            super().error(ErrorType.TYPE_ERROR, "parameter type mismatch")
        if param.t == Type.OBJECT and param.iface:
            self.__check_interface_satisfied(actual, param.iface)

    def __call_function_value(self, fvalue, arg_nodes, self_obj=None):
        if fvalue.t != Type.FUNCTION:
            super().error(ErrorType.TYPE_ERROR, "call on non-function")
        fv = fvalue.v
        if fv is None:
            super().error(ErrorType.FAULT_ERROR, "call on nil function")
        func_def = fv.func_def
        actual_args = [self.eval_expr(a) for a in arg_nodes]
        if len(actual_args) != len(func_def.params):
            super().error(ErrorType.TYPE_ERROR, "wrong arg count")
        self.callstack.append(CallContext(closure=fv.closure))
        self.env.enter_func()
        for param, actual in zip(func_def.params, actual_args):
            self.__check_arg_type_compat(actual, param)
            bound = self.__clone_for_passing(actual, param.is_ref)
            if not self.env.fdef(param.name, bound):
                super().error(ErrorType.NAME_ERROR, "parameter name conflict")
        if self_obj is not None:
            self.env.fdef("selfo", self_obj)
        res, _ = self.__run_statements(func_def, func_def.statements)
        self.env.exit_func()
        self.callstack.pop()
        return res

    def __run_if(self, funcdef, statement):
        cond = self.eval_expr(statement.get("condition"))
        if cond.t != Type.BOOL:
            super().error(ErrorType.TYPE_ERROR, "condition must be boolean")
        self.env.enter_block()
        res, ret = Value(funcdef.return_type), False
        if cond.v:
            res, ret = self.__run_statements(funcdef, statement.get("statements"))
        elif statement.get("else_statements"):
            res, ret = self.__run_statements(funcdef, statement.get("else_statements"))
        self.env.exit_block()
        return res, ret

    def __run_while(self, funcdef, statement):
        res, ret = Value(funcdef.return_type), False
        while True:
            cond = self.eval_expr(statement.get("condition"))
            if cond.t != Type.BOOL:
                super().error(ErrorType.TYPE_ERROR, "condition must be boolean")
            if not cond.v:
                break
            self.env.enter_block()
            res, ret = self.__run_statements(funcdef, statement.get("statements"))
            self.env.exit_block()
            if ret:
                break
        return res, ret

    def __run_return(self, funcdef, statement):
        expr = statement.get("expression")
        if not expr:
            default = Value(funcdef.return_type)
            return (default, True)
        result_val = self.eval_expr(expr)
        dest_t = funcdef.return_type
        dest_iface = funcdef.return_iface
        coerced = self.__coerce_assignment_type(dest_t, dest_iface, result_val)
        if dest_t == Type.OBJECT and dest_iface:
            self.__check_interface_satisfied(coerced, dest_iface)
        return (coerced, True)

    def __run_statements(self, funcdef, statements):
        res = Value(funcdef.return_type)
        ret = False
        for statement in statements:
            kind = statement.elem_type
            if kind == self.VAR_DEF_NODE:
                self.__run_vardef(statement)
            elif kind == self.BVAR_DEF_NODE:
                self.__run_vardef(statement, True)
            elif kind == "=":
                self.__run_assign(statement)
            elif kind == self.FCALL_NODE:
                self.__run_fcall(statement)
            elif kind == self.IF_NODE:
                res, ret = self.__run_if(funcdef, statement)
                if ret:
                    break
            elif kind == self.WHILE_NODE:
                res, ret = self.__run_while(funcdef, statement)
                if ret:
                    break
            elif kind == self.RETURN_NODE:
                res, ret = self.__run_return(funcdef, statement)
                break
        return res, ret

    def __eval_binary_op(self, kind, vl, vr):
        tl, tr = vl.t, vr.t
        vl_val, vr_val = vl.v, vr.v
        if kind == "==":
            if vl_val is None and vr_val is None:
                return Value(Type.BOOL, True)
            if tl == Type.FUNCTION and tr == Type.FUNCTION:
                return Value(Type.BOOL, vl_val is vr_val)
            if tl == Type.OBJECT and tr == Type.OBJECT:
                return Value(Type.BOOL, vl_val is vr_val)
            if tl != tr:
                return Value(Type.BOOL, False)
            return Value(Type.BOOL, vl_val == vr_val)
        if kind == "!=":
            if vl_val is None and vr_val is None:
                return Value(Type.BOOL, False)
            if tl == Type.FUNCTION and tr == Type.FUNCTION:
                return Value(Type.BOOL, not (vl_val is vr_val))
            if tl == Type.OBJECT and tr == Type.OBJECT:
                return Value(Type.BOOL, not (vl_val is vr_val))
            if tl != tr:
                return Value(Type.BOOL, True)
            return Value(Type.BOOL, vl_val != vr_val)
        if tl == Type.STRING and tr == Type.STRING and kind == "+":
            return Value(Type.STRING, vl_val + vr_val)
        if tl == Type.INT and tr == Type.INT:
            if kind == "+":
                return Value(Type.INT, vl_val + vr_val)
            if kind == "-":
                return Value(Type.INT, vl_val - vr_val)
            if kind == "*":
                return Value(Type.INT, vl_val * vr_val)
            if kind == "/":
                return Value(Type.INT, vl_val // vr_val)
            if kind == "<":
                return Value(Type.BOOL, vl_val < vr_val)
            if kind == "<=":
                return Value(Type.BOOL, vl_val <= vr_val)
            if kind == ">":
                return Value(Type.BOOL, vl_val > vr_val)
            if kind == ">=":
                return Value(Type.BOOL, vl_val >= vr_val)
        if tl == Type.BOOL and tr == Type.BOOL:
            if kind == "&&":
                return Value(Type.BOOL, vl_val and vr_val)
            if kind == "||":
                return Value(Type.BOOL, vl_val or vr_val)
        super().error(ErrorType.TYPE_ERROR, "invalid binary operation")

    def __eval_convert(self, expr):
        val = self.eval_expr(expr.get("expr"))
        to_type = expr.get("to_type")
        if to_type == "int":
            if val.t == Type.INT:
                return val
            if val.t == Type.STRING:
                try:
                    return Value(Type.INT, int(val.v))
                except ValueError:
                    super().error(ErrorType.TYPE_ERROR, "cannot convert string to int")
            if val.t == Type.BOOL:
                return Value(Type.INT, 1 if val.v else 0)
            super().error(ErrorType.TYPE_ERROR, "cannot convert to int")
        if to_type == "str":
            if val.t == Type.STRING:
                return val
            if val.t == Type.INT:
                return Value(Type.STRING, str(val.v))
            if val.t == Type.BOOL:
                return Value(Type.STRING, str(val.v).lower())
            super().error(ErrorType.TYPE_ERROR, "cannot convert to string")
        if to_type == "bool":
            if val.t == Type.BOOL:
                return val
            if val.t == Type.INT:
                return Value(Type.BOOL, val.v != 0)
            if val.t == Type.STRING:
                return Value(Type.BOOL, val.v != "")
            super().error(ErrorType.TYPE_ERROR, "cannot convert to bool")
        super().error(ErrorType.TYPE_ERROR, "invalid conversion type")

    def __get_var_value(self, expr):
        dotted = expr.get("name").split(".")
        base_name = dotted[0]
        if len(dotted) == 1:
            if self.env.env and self.env.exists(base_name):
                return self.env.get(base_name)
            if self.callstack and base_name in self.callstack[-1].closure:
                return self.callstack[-1].closure[base_name]
            if base_name in self.named_func_values:
                return Value(Type.FUNCTION, self.named_func_values[base_name])
            super().error(ErrorType.NAME_ERROR, "variable not defined")
        if self.env.env and self.env.exists(base_name):
            value = self.env.get(base_name)
        elif self.callstack and base_name in self.callstack[-1].closure:
            value = self.callstack[-1].closure[base_name]
        else:
            super().error(ErrorType.NAME_ERROR, "variable not defined")
        if value.t != Type.OBJECT:
            super().error(ErrorType.TYPE_ERROR, "cannot dereference a non-object")
        suffix = dotted[1:]
        for i, sub in enumerate(suffix):
            if value.v is None:
                super().error(ErrorType.FAULT_ERROR, "nil reference access")
            if sub not in value.v:
                super().error(ErrorType.NAME_ERROR, "object member not found")
            if i < len(suffix) - 1 and not (sub[-1] == "o" or sub[-1].isupper()):
                super().error(ErrorType.TYPE_ERROR, "member must be object/interface")
            value = value.v[sub]
        return value

    def __eval_lambda(self, expr):
        name = expr.get("name")
        last = name[-1]
        if not (last in "isbofv" or last.isupper()):
            super().error(ErrorType.TYPE_ERROR, "invalid lambda return type")
        func_def = FuncDef(expr, is_lambda=True)
        captures = {}
        if self.env.env:
            frames = self.env.env[-1]
            for block in reversed(frames):
                for k, v in block.items():
                    if k in captures:
                        continue
                    if v.t == Type.OBJECT:
                        captures[k] = Value(Type.OBJECT, v.v)
                    elif v.t == Type.FUNCTION:
                        captures[k] = Value(Type.FUNCTION, v.v)
                    else:
                        captures[k] = Value(v.t, v.v)
        if self.callstack:
            for k, v in self.callstack[-1].closure.items():
                if k in captures:
                    continue
                if v.t == Type.OBJECT:
                    captures[k] = Value(Type.OBJECT, v.v)
                elif v.t == Type.FUNCTION:
                    captures[k] = Value(Type.FUNCTION, v.v)
                else:
                    captures[k] = Value(v.t, v.v)
        fv = FuncValue(func_def, captures)
        return Value(Type.FUNCTION, fv)

    def eval_expr(self, expr):
        kind = expr.elem_type
        if kind == self.INT_NODE:
            return Value(Type.INT, expr.get("val"))
        if kind == self.STRING_NODE:
            return Value(Type.STRING, expr.get("val"))
        if kind == self.BOOL_NODE:
            return Value(Type.BOOL, expr.get("val"))
        if kind == self.NIL_NODE:
            return Value(Type.OBJECT)
        if kind == self.EMPTY_OBJ_NODE:
            return Value(Type.OBJECT, {})
        if kind == self.QUALIFIED_NAME_NODE:
            return self.__get_var_value(expr)
        if kind == self.FCALL_NODE:
            return self.__run_fcall(expr)
        if kind == self.FUNC_NODE:
            return self.__eval_lambda(expr)
        if kind in self.bops:
            l = self.eval_expr(expr.get("op1"))
            r = self.eval_expr(expr.get("op2"))
            return self.__eval_binary_op(kind, l, r)
        if kind == self.NEG_NODE:
            o = self.eval_expr(expr.get("op1"))
            if o.t == Type.INT:
                return Value(Type.INT, -o.v)
            super().error(ErrorType.TYPE_ERROR, "cannot negate non-integer")
        if kind == self.NOT_NODE:
            o = self.eval_expr(expr.get("op1"))
            if o.t == Type.BOOL:
                return Value(Type.BOOL, not o.v)
            super().error(ErrorType.TYPE_ERROR, "cannot apply NOT to non-boolean")
        if kind == self.CONVERT_NODE:
            return self.__eval_convert(expr)
        raise Exception("should not get here!")

    def __check_interface_satisfied(self, val, iface_name):
        if iface_name is None:
            return
        if val.t == Type.OBJECT and val.v is None:
            return
        if val.t != Type.OBJECT:
            super().error(ErrorType.TYPE_ERROR, "interface requires object")
        obj = val.v
        iface = self.interfaces.get(iface_name)
        if iface is None:
            super().error(ErrorType.NAME_ERROR, "unknown interface")
        for fname, (ftype, fiface) in iface.var_fields.items():
            if fname not in obj:
                super().error(ErrorType.TYPE_ERROR, "missing field in interface object")
            fval = obj[fname]
            if fval.t != ftype:
                super().error(ErrorType.TYPE_ERROR, "interface field type mismatch")
            if ftype == Type.OBJECT and fiface:
                self.__check_interface_satisfied(fval, fiface)
        for fname, fparams in iface.func_fields.items():
            if fname not in obj:
                super().error(ErrorType.TYPE_ERROR, "missing function in interface object")
            fval = obj[fname]
            if fval.t != Type.FUNCTION or fval.v is None:
                super().error(ErrorType.TYPE_ERROR, "interface requires function field")
            func_def = fval.v.func_def
            if len(func_def.params) != len(fparams):
                super().error(ErrorType.TYPE_ERROR, "interface function param mismatch")
            for actual_p, spec_p in zip(func_def.params, fparams):
                if actual_p.t != spec_p.t:
                    super().error(ErrorType.TYPE_ERROR, "interface function param type mismatch")
                if actual_p.iface != spec_p.iface:
                    super().error(ErrorType.TYPE_ERROR, "interface function param iface mismatch")
                if actual_p.is_ref != spec_p.is_ref:
                    super().error(ErrorType.TYPE_ERROR, "interface function param ref mismatch")

def main():
    import sys
    interpreter = Interpreter()
    filename = sys.argv[1] if len(sys.argv) > 1 else "./test.br"
    with open(filename, "r") as f:
        program = f.read()
    interpreter.run(program)

if __name__ == "__main__":
    main()
