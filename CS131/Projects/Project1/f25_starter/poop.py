from interpreterv1 import Interpreter

program = 'def main(){ var x; x = 5 + 6; print("The sum is ", x, "!"); }'
Interpreter().run(program)
# Expected: The sum is 11!
