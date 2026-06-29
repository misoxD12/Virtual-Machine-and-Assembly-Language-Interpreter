#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

//vmexception
//data structure


//Class: Register
//Purpose: Base class representing a single 8-bit signed register value.
//         Provides the get/set for every register need.
//Writer: Chai Ming Song
class Register{
protected:
    signed char value; //value stored in this register
public:
    Register(){ //constructor
        value = 0; //default starting value 0
    }

    virtual ~Register(){//destructor   
    }

    signed char getRegister() const{ //get the value hold by the register
        return value;
    }

    void setRegister(signed char newValue){ //set value for the register
        value = newValue;
    }
};

//Class: GeneralRegister
//Purpose: Represents R0–R7 registers.
//         Inherits from Register, and additionally remembers its own index.
//Writer: Chai Ming Song
class GeneralRegister : public Register{
    int index; //register index (0 for R0, up to 7 for R7)
public:
    GeneralRegister(int idx){ //constructor
        if (idx >= 0 && idx < 8){//check the index is within in the range
            index = idx;  //store index
        } else{
            throw InvalidRegisterException(idx); //invalid. error
        }
    }

    ~GeneralRegister(){ //destructor      
    }

    int getIndex() const{ //get which register 
        return index;
    }
};

//Class: FlagRegister
//Purpose: Holds the 4 flags (Carry, Zero, Underflow, Overflow) and provides the logic to check/reset/update them.
//Writer: Chai Ming Song
class FlagRegister{
        bool CarryFlag, ZeroFlag, UnderflowFlag, OverflowFlag; //the 4 flags,boolean
public:
    FlagRegister(){ //constructor
        CarryFlag = ZeroFlag = UnderflowFlag = OverflowFlag = false; //default init false
    }

    ~FlagRegister(){ //destructor
    }

    //getter and setter for flags
    bool getCarry() const{ //returns true if carry occurred
        return CarryFlag;
    }
    bool getZero() const{ //returns true if the result exactly 0
        return ZeroFlag;
    }
    bool getUnderflow() const{ //returns true if the result went below -128
        return UnderflowFlag;
    }
    bool getOverflow() const{//returns true if the result went above 127
        return OverflowFlag;
    }

    void setCarry(bool value){//directly set the carry flag
        CarryFlag = value;
    }
    void setZero(bool value){//directly set the zero flag
        ZeroFlag = value;
    }
    void setUnderflow(bool value){ //directly set the underflow flag
        UnderflowFlag = value;
    }
    void setOverflow(bool value){ //directly set the overflow flag
        OverflowFlag = value;
    }
    //reset flag by name
    void resetByName(string flagName){
        if (flagName == "CF"){
            CarryFlag = false;
        } else if (flagName == "ZF"){
            ZeroFlag = false;
        } else if (flagName == "UF"){
            UnderflowFlag = false;
        } else if (flagName == "OF"){
            OverflowFlag = false;
        } else {
            throw InvalidFlagException(flagName);
        }
    }
    //update flag result
    void updateFromResult(int result){
        if (result > 127){ //check overflow result too big for a signed byte  
            OverflowFlag = true;
        } else{
            OverflowFlag = false;
        }
        if (result < -128){ //check underflow result too small for a signed byte
            UnderflowFlag = true;
        } else{
            UnderflowFlag = false;
        }
        if (result == 0){ //check if the result exactly zero
            ZeroFlag = true;
        } else{
            ZeroFlag = false;
        }
        if (result > 255 || result < 0){//check carry result exceeds the capacity of 8 bits (unsigned 0-255 range)  
            CarryFlag = true;
        } else {
            CarryFlag = false;
        }
    }
};


//OperandType
//enum these type eg (R3, 5, [20], [R2])
enum OperandType {Register, Immediate, directMem, indirectMem};

////Class: Operand
//Purpose: A class that holds operand type, and value/register involved
//Writer: Chai Ming Song
class Operand{
    OperandType type; //what type of operand
    int regIndex;     //which register if register/ indirect mem
    int value;        //what value if immediate/ direct mem
public: 
    Operand(){ //constructor, default immediate with value 0
        type = Immediate;
        regIndex = -1;
        value = 0;
    }

    virtual ~Operand(){  // destructor
    }

    //getters
    OperandType getType() const{//get operand type
        return type;
    }
    int getRegIndex() const{//get the register index operand referring
        return regIndex;
    }
    int getValue() const{//get the plain num stored in the operand
        return value;
    }

    //setters
    void setType(OperandType t){//set operand type
        type = t;
    }
    void setRegIndex(int idx){//set register
        regIndex = idx;
    }
    void setValue(int v){//set the value
        value = v;
    }

    bool isValidRegister(string text){ //checks if text looks like a valid register name like "RNUM"
        if (text.length() == 2 && text [0] == 'R' && text [1] >= '0' && text [1] <= '7'){
            return true;
        } else{
            return false;
        }
    }

    //handles case where text has brackets
    Operand readBracketOperand(string text, int len){
        Operand op;
        //store the content in the brackets
        string inside = "";
        for (int i = 1; i < len-1; i++){//skit first and last "[" "]"
            inside = inside + text [i]; //add each character
        }

        if (isValidRegister(inside)){ //register inside bracket
            op.setType(indirectMem);
            int regNum = inside[1] - '0'; //turn the digit character into real number
            op.setRegIndex(regNum);
        } else{ //number inside bracket
            op.setType(directMem);
            int address = 0;
            for (int i = 0; i < (int)inside.length(); i++){
                address = address * 10 + (inside[i] - '0'); //build up the number digit by digit
            }
            op.setValue(address);
        }
        return op;
    }
    //handle plain immediate number
    Operand readImmediateOperand(string text, int len){
        Operand op;
        op.setType(Immediate);

        int startIndex = 0; //starting index at 0
        bool isNegative = false; //see whether the number end up negative
        if (text[0] == '-'){ //if first is a '-'
            startIndex = 1; //then skip '-'
            isNegative = true;
        }

        int num = 0;
        for (int i = startIndex; i < len; i++){
            num = num * 10 + (text[i] - '0'); //build up the number digit by digit
        }

        if(isNegative){
            num = -num; //flip the sign at the end
        }

        op.setValue(num);
        return op;
    }
    //Figure which operand type
    Operand readOperand(string text){
        int len = text.length();
        if(len == 0){
            throw MalformedOperandException("Empty Operand");//nothing to parse 
        };
        //check got brackets?
        bool hasBrackets = (text[0] == '[' && text[len - 1] == ']');

        if (hasBrackets) {
            return readBracketOperand(text, len);
        }

        if (isValidRegister(text)){
            Operand op;
            op.setType (Register);
            int regNum = text[1] - '0'; //turn the digit into num
            op.setRegIndex(regNum); //store the register
            return op;
        }

        if (text[0] == 'R' || text[0] == 'r') {//started with R or r but not a valid register
            throw MalformedOperandException(text);  
        }

        return readImmediateOperand(text, len);
    }
};

//memory

//Class: CPU
//Purpose: Holds all the vm components (registers, flags, memory,  stack, PC), and let instruction to read and change the values.
//Writer: Chai Ming Song
class CPU {
    GeneralRegister* registers[8]; //8 pointers r0-r7
    FlagRegister flags; 
    int PC; //program counter
    int SI; //stack index

    Memory memory; //the 64byte memory
    CustomStack <signed char> stack; //custom stack

public:
    CPU(){
        for (int i = 0; i < 8; i++){ //constructor to create all 8 registers, giving each one correct index
            registers[i] = new GeneralRegister(i);
        }

        PC = 0; //program counter start at 0
        SI = 0; //stack indes starts at 0
    }

    virtual ~CPU(){  //destructor to clean up all registers when CPU is destroyed
        for (int i = 0; i < 8; i++){
            delete registers[i]; //free the memory
        }
    }
    signed char getRegister(int idx) const{ //read the value stored in register idx (0-7)
        if (idx >=0 && idx < 8){
            return registers[idx] -> getRegister(); //fetch the value from register
        } else{
            throw InvalidRegisterException(idx); //invalid index 
        }
    }

    void setRegister(int idx, signed char value){ //change the value stored in in register 
        if (idx >=0 && idx < 8){
            registers[idx] -> setRegister(value); //write the new value into the register
        } else{
            throw InvalidRegisterException(idx); //invalid register
        }
    }

    FlagRegister &getFlags(){ //access to the FlagRegister so can check/update flags
        return flags;
    }

    int getPC() const{ //get current program counter value
        return PC;
    }

    void setPC(int value){//set program counter
        PC = value;
    }

    void incrementPC(){//increase program counter by 1 
        PC = PC + 1;
    }

    int getSI() const{ //read current stack index value
        return SI;
    }

    signed char getMemory(int address) const{ //read the value from memory at the given address
        return memory.readMemory(address);    
    }
    
    void setMemory(int address, signed char value){//write a value into memory at the given address
    memory.writeMemory(address, value);
}

    void pushValue(signed char value){//add value to the stack
        stack.push(value); //store in in the stack
        SI = SI + 1; //increase stack index by 1
    }

    signed char popValue(){ //remove the top of stack
        signed char v = stack.pop(); //take the top value off
        SI = SI - 1; //stack index minus 1
        return v;
        
    }
        
};

//Class: Instructions
//Purpose: Abstract base class for every single assembly instruction. Declares
//          execute() as pure virtual, so every concrete instruction must
//          provide its own version(polymorphsm)
//Writer: Chai Ming Song
class Instructions{
protected:
    int lineNum; //which line of the .asm file this instruction came from
public:
    Instructions(int line){ //constructor
        lineNum = line; //store the line number
    }
    virtual ~Instructions(){}//destructor

    virtual void execute(CPU &cpu) = 0; //every instruction must provide its own execute() (polumorphism)

    int getLineNum() const{  //get which line this instruction came from
        return lineNum;
    }

};

//oneoperand
//twooperand

//Class: ShiftInstruction
//Purpose: Shared base for ROL, ROR, SHL, SHR， managing the setup and unsigned data conversions.
//Writer: [Your Name]
class ShiftInstruction : public TwoOperandInstruction {
protected:
    int destReg; //destination register
    int count; //position count to shift/rotate

    //convert current value to unsigned
    unsigned char convertUnsigned(CPU &cpu){
        return static_cast<unsigned char>(cpu.getRegister(destReg));
    }

    //convert back the result to signed char and write it back into register
    void storeResult(CPU &cpu, unsigned char val){
        cpu.setRegister(destReg, static_cast<signed char>(val));
    }
public:
    ShiftInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2) {
        destReg = o1.getRegIndex(); //first operand to tell which register to work on
        count = o2.getValue(); //second operand to see how far to shift/rotate
        if (count < 0){  //a negative shift count makes no sense
            throw InvalidShiftCountException(count);
        }
    }

    virtual ~ShiftInstruction(){//destructor
    }
};

//Class: ROLCommand
//Purpose: This class rotates the bits of the destination register to the left by the specified count
//Writer: [Your Name]
class ROLCommand : public ShiftInstruction {
public:
    ROLCommand(int line, Operand o1, Operand o2) : ShiftInstruction(line, o1, o2) {//pass operands up to the shared base
    }

    void execute(CPU &cpu) override {
        unsigned char val = convertUnsigned(cpu); //get the register's value as unsigned
        int shiftCount   = count % 8; //reduce the count to the actual amount 
        val = (val << shiftCount) | (val >> (8 - shiftCount)); //shift left, then OR in the bits that fell off
        storeResult(cpu, val); //convert back to signed and write it back into the register
    }
};

//Class: RORCommand
//Purpose: This class rotates the bits of the destination register to the right by the specified count.
//Author: [Your Name]
class RORCommand : public ShiftInstruction {
public:
    RORCommand(int line, Operand o1, Operand o2) : ShiftInstruction(line, o1, o2) {
    }

    void execute(CPU &cpu) override {
        unsigned char val = convertUnsigned(cpu);
        int shiftCount = count % 8;
        val = (val >> shiftCount) | (val << (8 - shiftCount)); //Shift right, then OR in the bits that fell off
        storeResult(cpu, val);
    }
};

//Class: SHLCommand
//Purpose: This class shifts the bits of the destination register to the left by the specified count, filling with zeros.
//Author: [Your Name]
class SHLCommand : public ShiftInstruction {
public:
    SHLCommand(int line, Operand o1, Operand o2) : ShiftInstruction(line, o1, o2) {
    }

    void execute(CPU &cpu) override {
        unsigned char val = convertUnsigned(cpu);
        if (count >= 8) {  //if shift count more than 8 then zero everything
            val = 0; 
        } else {
            val = val << count; //shift left and padding right with zero
        }
        storeResult(cpu, val);
    }
};

//Class: SHRCommand
//Purpose: This class shifts the bits of the destination register to the right by the specified count, filling with zeros.
//Author: [Your Name]
class SHRCommand : public ShiftInstruction {
public:
    SHRCommand(int line, Operand o1, Operand o2) : ShiftInstruction(line, o1, o2) {
    }

    void execute(CPU &cpu) override {
        unsigned char val = convertUnsigned(cpu);
        if (count >= 8) {
            val = 0;
        } else {
            val = val >> count; //shift right and padding left with zero
        }
        storeResult(cpu, val);
    }
};