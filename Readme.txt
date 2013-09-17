DOCUMENTATION
=============
Load and run a program from command line: sandbox program.txt

Sandbox Instructions
load		- Asks user for name of program file to load and run

clear           - Clears the screen

clearmem        - Clears the sandbox memory

exit            - Exits the sandbox


ADD result var1 var2		  - Addition:       result = var1 + var2
SUB result var1 var2              - Subtraction:    result = var1 - var2
DIV result var1 var2              - Division:       result = var1 / var2
MUL result var1 var2              - Multiplication: result = var1 * var2
POW result var1 var2              - Raise to power: result = var1 ^ var2
SQRT result var1                  - Square root:    result = sqrt(var1)

CP var1 var2                      - Copy variable:  var1 = var2 (where var2 is a variable)
SET var1 var2                     - Sets var1:      var1 = var2 (where var2 can be a number or variable)

BEQ jump var1 var2                - If var1 == var2: stackPointer += jump
BNE jump var1 var2                - If var1 != var2: stackPointer += jump
BGE jump var1 var2                - If var1 >= var2: stackPointer += jump
BLE jump var1 var2                - If var1 <= var2: stackPointer += jump

PRINT var                         - Prints out the value of variable, var
