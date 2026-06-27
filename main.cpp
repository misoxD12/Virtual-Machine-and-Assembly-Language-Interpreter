#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

class Instructions{
protected:
    int lineNum;
public:
    Instructions(int line){ lineNum = line; }
    virtual ~Instructions(){}
    virtual void execute() = 0;
    int getLineNum() const{ return lineNum; }
};

class TwoOperandInstruction : public Instructions {
protected:
    string op1, op2;
public:
    TwoOperandInstruction(int line, string o1, string o2) : Instructions(line){
        op1 = o1;
        op2 = o2;
    }
    virtual ~TwoOperandInstruction(){}
    virtual void execute() = 0;
};

class MOVInstruction : public TwoOperandInstruction {
public:
    MOVInstruction(int line, string o1, string o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(){ /* TODO: integrate with CPU */ }
};

class ADDInstruction : public TwoOperandInstruction {
public:
    ADDInstruction(int line, string o1, string o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(){ /* TODO: integrate with CPU */ }
};

class SUBInstruction : public TwoOperandInstruction {
public:
    SUBInstruction(int line, string o1, string o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(){ /* TODO: integrate with CPU */ }
};

class MULInstruction : public TwoOperandInstruction {
public:
    MULInstruction(int line, string o1, string o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(){ /* TODO: integrate with CPU */ }
};

class DIVInstruction : public TwoOperandInstruction {
public:
    DIVInstruction(int line, string o1, string o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(){ /* TODO: HOLD - spec ambiguous */ }
};

class LOADInstruction : public TwoOperandInstruction {
public:
    LOADInstruction(int line, string o1, string o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(){ /* TODO: integrate with CPU */ }
};

class STOREInstruction : public TwoOperandInstruction {
public:
    STOREInstruction(int line, string o1, string o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(){ /* TODO: integrate with CPU */ }
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
    
    bool isRegister(string token){ 
        if (token.length() != 2) return false;
        if (token[0] != 'R') return false;     
        if (token[1] < '0' || token[1] > '7') return false; 
        return true;
    }
    
    bool isImmediate(string token){ 
        if (token.length() == 0) return false;
        if (token[0] != '-' && !isdigit(token[0])) return false;
        int start = (token[0] == '-') ? 1 : 0;
        if (start == 1 && token.length() == 1) return false;
        for (int i = start; i < token.length(); i++){
            if (!isdigit(token[i])) return false;
        }
        return true;
    }
    
    bool isBracketed(string token){
        return token[0] == '[' && token[token.length()-1] == ']';
    }
    
    string stripBrackets(string token){
        return token.substr(1, token.length()-2);
    }

    void executeInstruction(vector<string> instruction, int lineNum){
        string opcode = instruction[0];
        Instructions* instr = nullptr;
        
        string op1 = instruction.size() > 1 ? instruction[1] : "";
        string op2 = instruction.size() > 2 ? instruction[2] : "";

        // 2 operand - Simran
        if (opcode == "MOV") instr = new MOVInstruction(lineNum, op1, op2);
        else if (opcode == "ADD") instr = new ADDInstruction(lineNum, op1, op2);
        else if (opcode == "SUB") instr = new SUBInstruction(lineNum, op1, op2);
        else if (opcode == "MUL") instr = new MULInstruction(lineNum, op1, op2);
        else if (opcode == "DIV") instr = new DIVInstruction(lineNum, op1, op2);
        else if (opcode == "LOAD") instr = new LOADInstruction(lineNum, op1, op2);
        else if (opcode == "STORE") instr = new STOREInstruction(lineNum, op1, op2);
        
        // 1 operand - Person 3
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
            instr->execute();
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
        int registers[8] = {0, 11, 0, 44, 0, 0, 0, 0};
        int pc = 6;
        int memory[64] = {0};
        memory[20] = 44;

        cout << "#Begin#" << endl;
        cout << "#Registers#"; 
        for(int i=0; i<8; i++){
            cout << setfill('0') << setw(4) << registers[i] << "#";
        }
        cout << endl;

        int OF = 0, UF = 0, CF = 0, ZF = 0;

        cout << "#Flags#"
             << "OF#" << OF << "#"
             << "UF#" << UF << "#"
             << "CF#" << CF << "#"
             << "ZF#" << ZF << "#"
             << endl;
        
        cout << "#PC#" << setfill('0') << setw(4) << pc << "#" << endl;

        cout << "#Memory#" << endl;
        for(int i = 0; i < 8; i++){
            cout << "#";
            for(int j = 0; j < 8; j++){
                cout << setfill('0') << setw(4) << memory[i*8+j] << "#";
            }
            cout << endl;
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