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
//Class: TwoOperandInstruction 
// Purpose: thisclass for all instructions that take 2 operands
// Writer: Harsimran
class TwoOperandInstruction : public Instructions {
protected:
    Operand op1, op2; // op1 = destination, op2 = source
public:
    TwoOperandInstruction(int line, Operand o1, Operand o2) : Instructions(line){
        op1 = o1;
        op2 = o2;
    }
    virtual ~TwoOperandInstruction(){}
};
// Class: ArithmeticInstruction
// Puropuse: this class groups all arithmetic instructions  which is the ADD/SUB/MUL/DIV
// Writer: Harsimran
class ArithmeticInstruction : public TwoOperandInstruction {
public:
    ArithmeticInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    virtual ~ArithmeticInstruction(){}
};
// class: MOVInstruction
// Purpose: this class copy the value into destination register. Supports 4 modes which are immediate, register, indirect, direct
// Writer: Harsimran

class MOVInstruction : public TwoOperandInstruction {
public:
    MOVInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex(); // get destination register index
        if (op2.getType() == Immediate){
            // MOV R0, 10 -use for the immediate value directly
            cpu.setRegister(dest, (signed char)op2.getValue());
        }
        else if (op2.getType() == Register){
            // MOV R0, R1 - copy the value from one register to another one  
            cpu.setRegister(dest, cpu.getRegister(op2.getRegIndex()));
        }
        else if (op2.getType() == indirectMem){
            // MOV R0, [R1] -Register 1  value as the  memory address
            cpu.setRegister(dest, cpu.getMemory(cpu.getRegister(op2.getRegIndex())));
        }
        else if (op2.getType() == directMem){
            // MOV R0, [20] - read directly from memory address 20
            cpu.setRegister(dest, cpu.getMemory(op2.getValue()));
        }
        else{ throw InvalidOperandLogicException("MOV"); }
    }
};
// Class: ADDInstruction
// Purpose: this class add op2 to op1, stores in  op1 updates flags after operation
// Writer: Harsimran
class ADDInstruction : public ArithmeticInstruction {
public:
    ADDInstruction(int line, Operand o1, Operand o2) : ArithmeticInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex();
        int result; // use int to detect overflow before casting
        if (op2.getType() == Immediate){
            result = cpu.getRegister(dest) + op2.getValue();
        }
        else if (op2.getType() == Register){
            result = cpu.getRegister(dest) + cpu.getRegister(op2.getRegIndex());
        }
        else{ throw InvalidOperandLogicException("ADD"); }
        cpu.setRegister(dest, (signed char)result); // cast back to signed char
        cpu.getFlags().updateFromResult(result);// update the flaggs
    }
};

// Class: SUBInstruction 
// Purpose: this class subtract op2 to op1, stores in op1 updates flags after operation
// Writer: Harsimran
class SUBInstruction : public ArithmeticInstruction {
public:
    SUBInstruction(int line, Operand o1, Operand o2) : ArithmeticInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex();
        int result;
        if (op2.getType() == Immediate){
            result = cpu.getRegister(dest) - op2.getValue();
        }
        else if (op2.getType() == Register){
            result = cpu.getRegister(dest) - cpu.getRegister(op2.getRegIndex());
        }
        else{ throw InvalidOperandLogicException("SUB"); }
        cpu.setRegister(dest, (signed char)result);
        cpu.getFlags().updateFromResult(result); // update the flags
    }
};
// Class :MULInstruction
// Purpose: this class multiply op2 to op1, stores in op1 updates flags after operation
// Writer: Harsimran

class MULInstruction : public ArithmeticInstruction {
public:
    MULInstruction(int line, Operand o1, Operand o2) : ArithmeticInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex();
        int result;
        if (op2.getType() == Immediate){
            result = cpu.getRegister(dest) * op2.getValue();
        }
        else if (op2.getType() == Register){
            result = cpu.getRegister(dest) * cpu.getRegister(op2.getRegIndex());
        }
        else{ throw InvalidOperandLogicException("MUL"); }
        cpu.setRegister(dest, (signed char)result);
        cpu.getFlags().updateFromResult(result); // update the flags
    }
};
// Class: DIVInstruction
// Purpose: this class subtract op2 to op1, stores in op1 updates flags after operation
// Writer: Harsimran
class DIVInstruction : public ArithmeticInstruction {
public:
    DIVInstruction(int line, Operand o1, Operand o2) : ArithmeticInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex();
        int result;
        if (op2.getType() == Immediate){
            if (op2.getValue() == 0){ throw DivideByZeroException(); return; } //  check if got divide by zero 
            result = cpu.getRegister(dest) / op2.getValue();
        }
        else if (op2.getType() == Register){
            if (cpu.getRegister(op2.getRegIndex()) == 0){ throw DivideByZeroException(); return; }
            result = cpu.getRegister(dest) / cpu.getRegister(op2.getRegIndex());
        }
        else{ throw InvalidOperandLogicException("DIV"); return; }
        cpu.setRegister(dest, (signed char)result);
        cpu.getFlags().updateFromResult(result);
    }
};
// Class : LOADInstruction
// Purpose: this class loads value from memory into destination register and can use direct and indirect mode
// Writer: Harsimran
class LOADInstruction : public TwoOperandInstruction {
public:
    LOADInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex();
        if (op2.getType() == indirectMem){
            // LOAD R1, [R2] - to get the memery address from Register 2 
            cpu.setRegister(dest, cpu.getMemory(cpu.getRegister(op2.getRegIndex())));
        }
        else if (op2.getType() == directMem){
            //LOAD R1, [20] - read directly from memory address 20
            cpu.setRegister(dest, cpu.getMemory(op2.getValue()));
        }
        else{ throw InvalidOperandLogicException("LOAD"); }
    }
};
//Class: STOREInstruction 
// Purpose: this class stores register value into memory and support 3 modes
// Writer: Harsimran

class STOREInstruction : public TwoOperandInstruction {
public:
    STOREInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        if (op1.getType() == Register && op2.getType() == Immediate){
            // STORE R1, 43 - store the register 1 value at memory address 43
            cpu.setMemory(op2.getValue(), cpu.getRegister(op1.getRegIndex()));
        }
        else if (op1.getType() == Register && op2.getType() == indirectMem){
            //STORE R1, [R2] - store registrer one value at address stored in register 2 
            cpu.setMemory(cpu.getRegister(op2.getRegIndex()), cpu.getRegister(op1.getRegIndex()));
        }
        else if (op1.getType() == Immediate && op2.getType() == Register){
            // STORE 20, R3 - store register 3  value at address 20
            cpu.setMemory(op1.getValue(), cpu.getRegister(op2.getRegIndex()));
        }
        else{ throw InvalidOperandLogicException("STORE"); }
    }
};

//Class: ShiftInstruction
//Purpose: Shared base for ROL, ROR, SHL, SHR， managing the setup and unsigned data conversions.
//Writer: Chai Ming Song
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
//Writer: Chai Ming Song
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
//Writer: Chai Ming Song
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
//Writer: Chai Ming Song
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
//Writer: Chai Ming Song
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

// Class: FilterFromFile
// Purpose: this class cleans the raw lines from .asm file before parsing
// Writer: Harsimran
class FilterFromFile {
public:
    // Replaces all commas in a line with spaces
    string RemoveComma(string line){
        for (int i=0; i<line.length(); i++){
            if (line[i] == ',') {line[i] = ' ';}
        } return line;
    }

    // Split the cleaned line into individual words using whitespace as delimiter
    CustomVector<string> split(string FilteredLine) {
        CustomVector<string> words;
        stringstream ss(FilteredLine);
        string currentWord;
        while (ss >> currentWord) {
            words.push_back(currentWord);
        }
        return words;
    }
    //Convers all the charcters into upper case so more consistant
    string toUpperCase(string line){
        for (int i = 0; i < line.length(); i++){
            line[i] = toupper(line[i]);
        }
        return line;
    }
};

// Class: Parser 
// Purpose: Read and make the .asm file content into tokens
// Writer: Harsimran
class Parser {
private:
    FilterFromFile Filter;
    // checks if a token is a valid instruction opcode
    bool isOpcode(string token){
        string opcodes[] = {"MOV","ADD","SUB","MUL","DIV","INC","DEC","ROL","ROR","SHL","SHR","LOAD","STORE","RESET","PUSH","POP","INPUT","DISPLAY"};
        for (int i = 0; i < 18; i++){
            if (token == opcodes[i]) return true;
        }
        return false;
    }
public:
    Parser(){};
    // opens .asm file and then, cleans each line and then, split into token and return in 2d vecto 
    CustomVector<CustomVector<string>> FileOpening(string filename) {
        CustomVector<CustomVector<string>> FinalInstructions;
        string Lines;
        int lineNum = 0; 
        ifstream inputFromFile(filename);
        if (inputFromFile.fail()){
            throw FileIOException(filename); 
        }
        while(getline(inputFromFile, Lines)){
            lineNum++;
            string cleanLine = Filter.toUpperCase(Filter.RemoveComma(Lines));
            CustomVector<string> ExtractedWords = Filter.split(cleanLine);
            
            if (ExtractedWords.size() > 0){ // check if more than one instruction on same line, cuz according to pdf must exit 
                for (int i = 1; i < ExtractedWords.size(); i++){
                    if (isOpcode(ExtractedWords[i])){
                        throw MultipleInstructionsException(lineNum); 

                    }
                }
                FinalInstructions.push_back(ExtractedWords);
            }
        }

        inputFromFile.close();
        return FinalInstructions;
    }
};

//Class : Runner 
// Purpouse: this class executes the .asm program and dumps the VM state
// Writer: Harsimran
class Runner {
private:
    Parser parser;
    CustomQueue<CustomVector<string>> program; /// queue stores instructions in order
    CPU cpu;
    // create the 1 operand instruction object based on opcode that we extract from the file
    Instructions* createOneOperand(string opcode, int lineNum, CustomVector<string> instruction){
        Operand operandParser;
        if (opcode == "RESET") return new RESETInstruction(lineNum, instruction[1]);
        Operand opnd = operandParser.readOperand(instruction[1]);
        if (opcode == "INC") return new INCInstruction(lineNum, opnd);
        else if (opcode == "DEC") return new DECInstruction(lineNum, opnd);
        else if (opcode == "PUSH") return new PUSHInstruction(lineNum, opnd);
        else if (opcode == "POP") return new POPInstruction(lineNum, opnd);
        else if (opcode == "INPUT") return new INPUTInstruction(lineNum, opnd);
        else if (opcode == "DISPLAY") return new DISPLAYInstruction(lineNum, opnd);
        return nullptr;
    }
    // create the 2 operand instruction object based on opcode that we extract from the file
    Instructions* createTwoOperand(string opcode, int lineNum, Operand op1, Operand op2){
        if (opcode == "MOV") return new MOVInstruction(lineNum, op1, op2);
        else if (opcode == "ADD") return new ADDInstruction(lineNum, op1, op2);
        else if (opcode == "SUB") return new SUBInstruction(lineNum, op1, op2);
        else if (opcode == "MUL") return new MULInstruction(lineNum, op1, op2);
        else if (opcode == "DIV") return new DIVInstruction(lineNum, op1, op2);
        else if (opcode == "LOAD") return new LOADInstruction(lineNum, op1, op2);
        else if (opcode == "STORE") return new STOREInstruction(lineNum, op1, op2);
        else if (opcode == "ROL") return new ROLCommand(lineNum, op1, op2);
        else if (opcode == "ROR") return new RORCommand(lineNum, op1, op2);
        else if (opcode == "SHL") return new SHLCommand(lineNum, op1, op2);
        else if (opcode == "SHR") return new SHRCommand(lineNum, op1, op2);
        return nullptr;
    }
    // runs the opcode and executes the correct instruction 
    void executeInstruction(CustomVector<string> instruction, int lineNum){
        string opcode = instruction[0];
        Instructions* instr = nullptr;
        Operand operandParser;
        Operand op1, op2;  

        bool isOneOperand = false;
        if (opcode == "RESET" || opcode == "INC" || opcode == "DEC" || opcode == "PUSH" || opcode == "POP" || opcode == "INPUT" ||opcode == "DISPLAY"){// check if its a 1 operand instruction
            isOneOperand = true;
        }

        if (isOneOperand){
            instr = createOneOperand(opcode, lineNum, instruction);
        } else {


            // if not 1 operand then should be 2, and 2 operand needs at least 3 tokens which is the [opcode + 2 operands] 
            if (instruction.size() < 3){
                throw MalformedOperandException("missing operand(s) for " + opcode);
            }
            op1 = operandParser.readOperand(instruction[1]);
            op2 = operandParser.readOperand(instruction[2]);
            instr = createTwoOperand(opcode, lineNum, op1, op2);
            if (instr == nullptr){
                throw UnknownInstructionException(opcode);
            }
        }
        if (instr != nullptr){
            instr->execute(cpu);
            delete instr;
        }
    }
public:

    void load(string filename){// load the file into program queue
        CustomVector<CustomVector<string>> parsed = parser.FileOpening(filename);
        for (int i = 0; i < parsed.size(); i++){
            program.enqueue(parsed[i]);
        }
    }

    void run(){
        int lineNum = 0;
        while (!program.isEmpty()){
            executeInstruction(program.dequeue(), lineNum); //executes the instructions in the queue one by one 
            cpu.incrementPC();
            lineNum++;
        }
    }
    // write text to both terminal and output file
    void writeLineToOutputFile(ofstream &ToFile, string text){
        cout << text << endl;
        ToFile << text << endl;
    }

    // formats the intiger value to  string based on width
    string formatVal(int val, int width){
        string digits = to_string(abs(val));
        while((int)digits.length() < width) digits = "0" + digits;
        if (val < 0){
            return "-" + digits;
        } else {
            return digits;
        }
    }

    void dump(){
        // prints final  state of our VM to terminal and output.txt as the output file 
        ofstream ToFile("output.txt");
        if (ToFile.fail()){ throw FileIOException("output.txt"); return; }

        writeLineToOutputFile(ToFile, "#Begin#");

        string regLine = "#Registers#"; //print all 8 registers
        for(int i = 0; i < 8; i++){
            int val = (int)cpu.getRegister(i);
            if (val < 0){
                regLine += formatVal(val, 3) + "#";
            } else {
                regLine += formatVal(val, 4) + "#";
            }
        }
        writeLineToOutputFile(ToFile, regLine);

        string flagLine = "#Flags#OF#" + to_string(cpu.getFlags().getOverflow()) +///print 4 flags
                        "#UF#" + to_string(cpu.getFlags().getUnderflow()) +
                        "#CF#" + to_string(cpu.getFlags().getCarry()) +
                        "#ZF#" + to_string(cpu.getFlags().getZero()) + "#";
        writeLineToOutputFile(ToFile, flagLine);

        writeLineToOutputFile(ToFile, "#PC#" + formatVal(cpu.getPC(), 4) + "#"); // print program counter PC 

        writeLineToOutputFile(ToFile, "#Memory#"); // print the 64 memory as 8 rows of 8 like in pdf one 
        for(int i = 0; i < 8; i++){
            string memLine = "#";
            for(int j = 0; j < 8; j++){
                int val = (int)cpu.getMemory(i*8+j);
                if (val < 0){
                    memLine += formatVal(val, 3) + "#";
                } else {
                    memLine += formatVal(val, 4) + "#";
                }
            }
            writeLineToOutputFile(ToFile, memLine);
        }

        writeLineToOutputFile(ToFile, "#End#");
        ToFile.close();
    }
};

int main(){ 
    Runner runner;
    runner.load("assembly.asm");
    runner.run();
    runner.dump(); 
    return 0;
}