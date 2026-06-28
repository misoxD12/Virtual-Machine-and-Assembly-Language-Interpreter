*using sstream for parsing* fixed 

From assignment guide:-
Your virtual machine architecture will include the following components:
• Data Registers: 8 general-purpose data registers (R0–R7), each 1 byte (signed char, -128 to
127).
• Program Counter (PC): 1 byte, starts at 0, increments after each instruction.
• Stack Index (SI) Register: 1 byte, starts at 0, incremented when items are pushed to the stack
and decremented when an item is popped from the stack.
• Flags: 4 flag bits (Overflow, Underflow, Carry, Zero).
• Memory: 1-dimensional array of 64 signed bytes (addresses 0–63).
• Runner (Interpreter): Reads .asm files, decodes instructions, and executes them via the
CPU class.


*use assuption to hardcode while dump()* - fixed 

dump func not working as supposed to - Fixed 
------------------------------------------------------------------------------------------------------------------------------------------------------
For asm file (22 lines)

MOV R0, 15
MOV R1, -10
ADD R0, R1
STORE 5, R0
MOV R2, 30
MUL R2, R0
STORE 10, R2
SUB R3, R1
INC R3
DEC R0
MOV R4, 7
ROL R4, 2
MOV R5, -112
SHR R5, 2
PUSH R2
PUSH R3
POP R6
POP R7
LOAD R4, [5]
LOAD R5, [10]
RESET CF
DISPLAY R2


Expected Outcome 

-106 <-- this one the display func that happens before the dump()
#Begin#
#Registers#0004#-010#-106#0011#0005#-106#0011#-106#
#Flags#OF#1#UF#0#CF#0#ZF#0#
#PC#0022#
#Memory#
#0000#0000#0000#0000#0000#0005#0000#0000#
#0000#0000#-106#0000#0000#0000#0000#0000#
... rest zeros
#End#