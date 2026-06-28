#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

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
        for (int i = 0; i < 8; i++){
            delete registers[i];
        }
    }
    signed char getRegister(int idx) const{ //read the value stored in register idx (0-7)
        if (idx >=0 && idx < 8){
            return registers[idx] -> getRegister();
        } else{
            cerr << "invalid register number" << endl;
            return 0;
        }
    }

    void setRegister(int idx, signed char value){
        if (int i = 0; i < 8){
            registers[idx] -> setRegister(value);
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
};

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

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class TwoOperandInstruction : public Instructions {
protected:
    Operand op1, op2;
public:
    TwoOperandInstruction(int line, Operand o1, Operand o2) : Instructions(line){
        op1 = o1;
        op2 = o2;
    }
    virtual ~TwoOperandInstruction(){}
    virtual void execute(CPU &cpu) = 0;
};

class MOVInstruction : public TwoOperandInstruction {
public:
    MOVInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex();
        if (op2.getType() == Immediate){
            cpu.setRegister(dest, (signed char)op2.getValue());
        }
        else if (op2.getType() == Register){
            cpu.setRegister(dest, cpu.getRegister(op2.getRegIndex()));
        }
        else if (op2.getType() == indirectMem){
            // TODO: cpu.setRegister(dest, cpu.getMemory(cpu.getRegister(op2.getRegIndex())));
        }
        else if (op2.getType() == directMem){
            // TODO: cpu.setRegister(dest, cpu.getMemory(op2.getValue()));
        }
        else{ cerr << "Invalid MOV operand!" << endl; }
    }
};

class ADDInstruction : public TwoOperandInstruction {
public:
    ADDInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex();
        int result;
        if (op2.getType() == Immediate){
            result = cpu.getRegister(dest) + op2.getValue();
        }
        else if (op2.getType() == Register){
            result = cpu.getRegister(dest) + cpu.getRegister(op2.getRegIndex());
        }
        else{ cerr << "Invalid ADD operand!" << endl; }
        cpu.setRegister(dest, (signed char)result);
        cpu.getFlags().updateFromResult(result);
    }
};

class SUBInstruction : public TwoOperandInstruction {
public:
    SUBInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex();
        int result;
        if (op2.getType() == Immediate){
            result = cpu.getRegister(dest) - op2.getValue();
        }
        else if (op2.getType() == Register){
            result = cpu.getRegister(dest) - cpu.getRegister(op2.getRegIndex());
        }
        else{ cerr << "Invalid SUB operand!" << endl; }
        cpu.setRegister(dest, (signed char)result);
        cpu.getFlags().updateFromResult(result);
    }
};

class MULInstruction : public TwoOperandInstruction {
public:
    MULInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex();
        int result;
        if (op2.getType() == Immediate){
            result = cpu.getRegister(dest) * op2.getValue();
        }
        else if (op2.getType() == Register){
            result = cpu.getRegister(dest) * cpu.getRegister(op2.getRegIndex());
        }
        else{ cerr << "Invalid MUL operand!" << endl; }
        cpu.setRegister(dest, (signed char)result);
        cpu.getFlags().updateFromResult(result);
    }
};

class DIVInstruction : public TwoOperandInstruction {
public:
    DIVInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        // TODO: HOLD - spec ambiguous on division order
        cerr << "DIV not yet implemented" << endl;
    }
};

class LOADInstruction : public TwoOperandInstruction {
public:
    LOADInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex();
        if (op2.getType() == indirectMem){
            // LOAD R1, [R2]
            // TODO: cpu.setRegister(dest, cpu.getMemory(cpu.getRegister(op2.getRegIndex())));
        }
        else if (op2.getType() == directMem){
            // LOAD R1, [20]
            // TODO: cpu.setRegister(dest, cpu.getMemory(op2.getValue()));
        }
        else{ cerr << "Invalid LOAD operand!" << endl; }
    }
};

class STOREInstruction : public TwoOperandInstruction {
public:
    STOREInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        if (op1.getType() == Register && op2.getType() == Immediate){
            // STORE R1, 43
            // TODO: cpu.setMemory(op2.getValue(), cpu.getRegister(op1.getRegIndex()));
        }
        else if (op1.getType() == Register && op2.getType() == indirectMem){
            // STORE R1, [R2]
            // TODO: cpu.setMemory(cpu.getRegister(op2.getRegIndex()), cpu.getRegister(op1.getRegIndex()));
        }
        else if (op1.getType() == directMem && op2.getType() == Register){
            // STORE 20, R2
            // TODO: cpu.setMemory(op1.getValue(), cpu.getRegister(op2.getRegIndex()));
        }
        else{ cerr << "Invalid STORE operand!" << endl; }
    }
};

class FilterFromFile {
public:
    string RemoveComma(string line){
        for (int i=0; i<line.length(); i++){
            if (line[i] == ',') {line[i] = ' ';}
        } return line;
    }
    
    vector<string> split(string FilteredLine) {
        vector<string> words;
        stringstream ss(FilteredLine);
        string currentWord;
        while (ss >> currentWord) {
            words.push_back(currentWord);
        }
        return words;
    }

    string toUpperCase(string line){
        for (int i = 0; i < line.length(); i++){
            line[i] = toupper(line[i]);
        }
        return line;
    }
};

class Parser {
private:
    FilterFromFile Filter;
public:
    Parser(){};
    
    vector<vector<string>> FileOpening(string filename) {
        vector<vector<string>> FinalInstructions;
        string Lines;
        ifstream inputFromFile(filename);
        if (inputFromFile.fail()){
            cerr << "Error in opening the file" << endl; 
            return FinalInstructions;
        } else {
            while(getline(inputFromFile, Lines)){
                string cleanLine = Filter.toUpperCase(Filter.RemoveComma(Lines));
                vector<string> ExtractedWords = Filter.split(cleanLine);
                if (ExtractedWords.size() > 0){
                    FinalInstructions.push_back(ExtractedWords);
                }
            }
        }
        inputFromFile.close();
        return FinalInstructions;
    }
};

class Runner {
private:
    Parser parser;
    vector<vector<string>> program;
    CPU cpu;

    void executeInstruction(vector<string> instruction, int lineNum){
    string opcode = instruction[0];
    Instructions* instr = nullptr;
    
    Operand operandParser;
    Operand op1, op2;
    
    if (instruction.size() > 1) op1 = operandParser.readOperand(instruction[1]);
    if (instruction.size() > 2) op2 = operandParser.readOperand(instruction[2]);

    if (opcode == "MOV") instr = new MOVInstruction(lineNum, op1, op2);
    else if (opcode == "ADD") instr = new ADDInstruction(lineNum, op1, op2);
    else if (opcode == "SUB") instr = new SUBInstruction(lineNum, op1, op2);
    else if (opcode == "MUL") instr = new MULInstruction(lineNum, op1, op2);
    else if (opcode == "DIV") instr = new DIVInstruction(lineNum, op1, op2);
    else if (opcode == "LOAD") instr = new LOADInstruction(lineNum, op1, op2);
    else if (opcode == "STORE") instr = new STOREInstruction(lineNum, op1, op2);
    
    else if (opcode == "INC") { /* TODO: Person 3 */ }
    else if (opcode == "DEC") { /* TODO: Person 3 */ }
    else if (opcode == "PUSH") { /* TODO: Person 3 */ }
    else if (opcode == "POP") { /* TODO: Person 3 */ }
    else if (opcode == "INPUT") { /* TODO: Person 3 */ }
    else if (opcode == "DISPLAY") { /* TODO: Person 3 */ }
    else if (opcode == "RESET") { /* TODO: Person 3 */ }
    
    else {
        cerr << "Unknown instruction: " << opcode << endl;
        exit(1);
    }

    if (instr != nullptr){
        instr->execute(cpu);
        delete instr;
    }
    }

public:
    void load(string filename){
        program = parser.FileOpening(filename);
    }
    
    void run(){
        for (int i = 0; i < program.size(); i++){
            executeInstruction(program[i], i);
        }
    }

    void dump(){
        cout << "#Begin#" << endl;
        cout << "#Registers#"; 
        for(int i=0; i<8; i++){
            int regVal = (int)cpu.getRegister(i);
            if (regVal < 0){
                cout << "-" << setfill('0') << setw(3) << abs(regVal) << "#";
            } else {
                cout << setfill('0') << setw(4) << regVal << "#";
            }
        }
        cout << endl;

        int OF = 0, UF = 0, CF = 0, ZF = 0;

        cout << "#Flags#"
             << "OF#" << cpu.getFlags().getOverflow() << "#"
             << "UF#" << cpu.getFlags().getUnderflow() << "#"
             << "CF#" << cpu.getFlags().getCarry() << "#"
             << "ZF#" << cpu.getFlags().getZero() << "#"
             << endl;
        
        cout << "#PC#" << setfill('0') << setw(4) << cpu.getPC() << "#" << endl;

        cout << "#Memory#" << endl;
        for(int i = 0; i < 8; i++){
            cout << "#";
            for(int j = 0; j < 8; j++){
                                //cout << setfill('0') << setw(4) << memory[i*8+j] << "#";
            }
            cout << endl;
            cout << "Removed Hardcoded, will manually add in next" << endl;
        }
        cout << "#End#" << endl;
    }
};

int main(){
    Runner runner;
    runner.load("assembly.asm");
    runner.run();
    runner.dump(); 
    return 0;
}