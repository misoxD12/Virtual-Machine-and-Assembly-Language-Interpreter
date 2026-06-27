#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;


//data structure

//registers
class Register{
protected:
    signed char value; //num store in this reg
public:
    Register(){ //constructor start reg at 0
        value = 0;
    }

    virtual ~Register(){   
    }

    signed char getRegister() const{ //get current value hold by the reg
        return value;
    }

    void setRegister(signed char newValue){ //set new value for the reg
        value = newValue;
    }
};

//represent r0 - r7
class GeneralRegister : public Register{
    int index;
public:
    GeneralRegister(int idx){ //constructor assign id  (0-7)
        if (idx >= 0 && idx < 8){
            index = idx;
        } else{
            cerr << "Invalid register number" << endl;
            index = -1;
        }
    }

    ~GeneralRegister(){   
    }

    int getIndex() const{ //get which register 
        return index;
    }
};

//4 Flags
class FlagRegister{
        bool CarryFlag, ZeroFlag, UnderflowFlag, OverflowFlag;
public:
    FlagRegister(){ //constructor init false
        CarryFlag = ZeroFlag = UnderflowFlag = OverflowFlag = false;
    }

    ~FlagRegister(){ //destructor
    }

    //getter and setter for flags
    bool getCarry() const{
        return CarryFlag;
    }
    bool getZero() const{
        return ZeroFlag;
    }
    bool getUnderflow() const{
        return UnderflowFlag;
    }
    bool getOverflow() const{
        return OverflowFlag;
    }

    void setCarry(bool value){
        CarryFlag = value;
    }
    void setZero(bool value){
        ZeroFlag = value;
    }
    void setUnderflow(bool value){
        UnderflowFlag = value;
    }
    void setOverflow(bool value){
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
            cerr << "Unknown flag name" << endl;
        }
    }
    //update flag result
    void updateFromResult(int result){
        if (result > 127){
            OverflowFlag = true;
        } else{
            OverflowFlag = false;
        }
        if (result < -128){
            UnderflowFlag = true;
        } else{
            UnderflowFlag = false;
        }
        if (result == 0){
            ZeroFlag = true;
        } else{
            ZeroFlag = false;
        }
        if (result > 255 || result < 0){
        CarryFlag = true;
        } else{
        CarryFlag = false;
        }
    }
};

//to be update
//int result = R[destination] + R[source];
//flags.updateFromResult(result);

/** 
int result = registers[destination]->get() + registers[source]->get();
flags.updateFromResult(result);
**/

//operand
//number these type
enum OperandType {Register, Immediate, directMem, indirectMem};

//to hold operand type and what value/register it involve
class Operand{
    OperandType type; //what type
    int regIndex;     //which register if register/ indirect mem
    int value;        //what value if immediate/ direct mem
public: 
    Operand(){ //constructor, default immediate with value 0
        type = Immediate;
        regIndex = -1;
        value = 0;
    }

    virtual ~Operand(){   // destructor
    }

    //getters
    OperandType getType() const{ //get operand type
        return type;
    }
    int getRegIndex() const{
        return regIndex;
    }
    int getValue() const{
        return value;
    }

    //setters
    void setType(OperandType t){
        type = t;
    }
    void setRegIndex(int idx){
        regIndex = idx;
    }
    void setValue(int v){
        value = v;
    }

    bool isValidRegister(string text){ //checks if text looks like a valid register name like RNUM
        if (text.length() == 2 && text [0] == 'R' && text [1] >= '0' && text [1] <= '7'){
            return true;
        } else{
            return false;
        }
    }

    //read operand and figure which operand type it is
    Operand readOperand(string text){
        Operand op;
        int len = text.length();
        
        //check got brackets?
        bool hasBrackets = (text[0] == '[' && text[len - 1] == ']');

        if (hasBrackets) {
            //store the content in the brackets
            string inside = "";
            for (int i = 1; i < len-1; i++){
                inside = inside + text [i];
            }

            if (isValidRegister(inside)){
                op.setType(indirectMem);
                int regNum = inside[1] - 1 - '0';
                op.setRegIndex(regNum);
            } else{
                op.setType(directMem);
                int address = 0;
                for (int i = 0; i < (int)inside.length(); i++){
                    address = address * 10 + (inside[i] - '0');
                }
                op.setValue(address);
            }
            return op;
        }

        if (isValidRegister(text)){
            op.setType (Register);
            int regNum = text[1] - '0';
            op.setRegIndex(regNum);
            return op;
        }

        op.setType(Immediate);
        int num = 0;
        for (int i = 0; i < len; i++){
            num = num * 10 + (text[i] - '0');
        }
        op.setValue(num);
        return op;
    }
};


//memory


//instructions
class Instructions{
protected:
    int lineNum;
public:
    Instructions(int line){ //constructor
        lineNum = line;
    }

    virtual ~Instructions(){
    }

    virtual void execute(CPU &cpu) = 0; //for instrcution polymorphsm

    int getLineNum() const{
        return lineNum;
    }

};

//class OneOperandInstructions : public Instructions { janine

//class TwoOperandIntsturctions : public Instructions { SIMRAN

//cpu
class CPU {
    GeneralRegister* registers[8]; //8 pointers r0-r7
    FlagRegister flags;
    int PC;
    int SI;

    //placeholder 
    //Memory memory;

    //placeholder
    //Stack <signed char> stack;
public:
    CPU(){
        for (int i = 0; i < 8; i++){ //constructor to create all 8 registers, giving each one its correct index
            registers[i] = new GeneralRegister(i);
        }

        PC = 0;
        SI = 0;
    }

    virtual ~CPU(){  //destructor to clean up all registers when CPU is destroyed
        for (int i = 0; i < 8){
            delete registers[i];
        }
    }
    signed char getRegister(int idx) const{ //read the value stored in register idx (0-7)
        if (idx >=0 && idx <= 8){
            return registers[idx] -> get();
        } else{
            cerr << "invalid register number" << endl;
            return 0;
        }
    }

    void setRegister(int idx, signed char value){
        if (int i = 0; i < 8){
            registers[idx] -> set(value);
        } else{
            cerr << "invalid register number" << endl;
        }
    }

    FlagRegister &getFlags(){ //access to the FlagRegister so can check/update flags
        return flags;
    }

    int getPC() const{ //program counter loh
        return PC;
    }

    void setPC(int value){
        PC = value;
    }

    void incrementPC(){
        PC = PC + 1;
    }

    int getSI() const{ //stack index loh
        return SI;
    }
    /*
    // PLACEHOLDER 
    signed char getMemory(int address) const{
        return memory.readByte(address);
    }

    // PLACEHOLDER 
    void pushValue(signed char value){
        stack.push(value);
        SI = SI + 1;
    }

    signed char popValue(){
        signed char v = stack.pop(){
            SI = SI - 1;
            return v;
        }
    }
        */
}；



//pasrser

//runner

// exception




int main(){
    cout<<"";
    return 0;
    
}