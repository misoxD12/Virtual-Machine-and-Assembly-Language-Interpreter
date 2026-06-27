#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
//#include <cstdint>
// simran so handsome 
using namespace std;


//data structure

//registers

//memory

//operand??
//instructions

//cpu

//parser

class FilterFromFile {
public:
    string RemoveComma(string line){
        for (int i=0; i<line.length(); i++){
            if (line[i] == ',') {line[i] = ' ';};
        } return line;
    }
    
    vector<string> split(string FilteredLine) {
        vector<string> words;
        stringstream ss(FilteredLine);
        string currentWord;
        
        // The >> operator automatically skips all whitespace!
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
    int length = 0;
    FilterFromFile Filter;
public:
    Parser() {};
    
    vector< vector<string> > FileOpening(string filename) {
        vector< vector<string> > FinalInstructions;
        string Lines;
        // open file here 
        ifstream inputFromFile(filename);
        // then we see if ok or not
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

//runner
class Runner {
private:
    Parser parser;
    vector<vector<string>> program; 
    
    bool isRegister(string instruction){ 
        if (instruction.length() != 2) return false;
        if (instruction[0] != 'R') return false;     
        if (instruction[1] < '0' || instruction[1] > '7') return false; 
        return true;
    }
    
    bool isImmediate(string instruction){ 
        int start = (instruction[0] == '-') ? 1 : 0;  // skip '-' if negative number
        if (start == 1 && instruction.length() == 1) return false; // just "-" alone is invalid
        for (int i = start; i < instruction.length(); i++){
            if (!isdigit(instruction[i])) return false;  // if any char not a digit, not a number
        }
        return true;
    }
    
    bool isBracketed(string instruction){
        if (instruction[0] == '[' && instruction[instruction.length()-1] == ']'){return true;}
        else {return false;}
    }
    
    string stripBrackets(string instruction){
        string stripped = instruction.substr(1, instruction.length()-2);
        return stripped; 
    }

    void executeMOV(vector<string> instruction){
    // check operand count first
    if (instruction.size() != 3){
        cerr << "MOV expects 2 operands" << endl;
        exit(1);
    }
    
    string op1 = instruction[1]; // destination register e.g "R0"
    string op2 = instruction[2]; // source e.g "10", "R1", "[R1]"
    
    if (isImmediate(op2)){
        int value = stoi(op2);
        // TODO: cpu.setRegister(op1, value)
    }
    else if (isRegister(op2)){
        // TODO: cpu.setRegister(op1, cpu.getRegister(op2))
    }
    else if (isBracketed(op2)){
        string inner = stripBrackets(op2);
        // TODO: cpu.setRegister(op1, cpu.getMemory(cpu.getRegister(inner)))
    }
    else{
        cerr << "Invalid MOV operand" << endl;
        exit(1);
    }
    }

    void executeADD(vector<string> instruction){
    // check operand count first
    if (instruction.size() != 3){
        cerr << "ADD expects 2 operands" << endl;
        exit(1);
    }
    
    string op1 = instruction[1]; // destination register e.g "R0"
    string op2 = instruction[2]; // source e.g "10", "R1", "[R1]"
    
    if (isImmediate(op2)){
        int value = stoi(op2);
        // TODO: cpu.setRegister(op1, value)
            // TODO: cpu.updateFlags(result)

    }
    else if (isRegister(op2)){
        // TODO: cpu.setRegister(op1, cpu.getRegister(op2))
            // TODO: cpu.updateFlags(result)

    }
    else{
        cerr << "Invalid ADD operand" << endl;
        exit(1);
    }
    }
    void executeSUB(vector<string> instruction){
    // check operand count first
    if (instruction.size() != 3){
        cerr << "SUB expects 2 operands" << endl;
        exit(1);
    }
    
    string op1 = instruction[1]; // destination register e.g "R0"
    string op2 = instruction[2]; // source e.g "10", "R1", "[R1]"
    
    if (isImmediate(op2)){
        int value = stoi(op2);
        // SUB todos should be:
        // TODO: cpu.setRegister(op1, cpu.getRegister(op1) - value)

}
else if (isRegister(op2)){
        // TODO: cpu.setRegister(op1, cpu.getRegister(op1) - cpu.getRegister(op2))
        
    }
    else{
        cerr << "Invalid SUB operand" << endl;
        exit(1);
    }
    }
    void executeMUL(vector<string> instruction){
    // check operand count first
    if (instruction.size() != 3){
        cerr << "MUL expects 2 operands" << endl;
        exit(1);
    }
    
    string op1 = instruction[1]; // destination register e.g "R0"
    string op2 = instruction[2]; // source e.g "10", "R1", "[R1]"
    
    if (isImmediate(op2)){
        int value = stoi(op2);
        // MUL todos should be:
// TODO: cpu.setRegister(op1, cpu.getRegister(op1) * value)

    }
    else if (isRegister(op2)){
// TODO: cpu.setRegister(op1, cpu.getRegister(op1) * cpu.getRegister(op2))

    }
    else{
        cerr << "Invalid MUL operand" << endl;
        exit(1);
    }
    }
    void executeDIV(vector<string> instruction){
    // check operand count first
    if (instruction.size() != 3){
        cerr << "DIV expects 2 operands" << endl;
        exit(1);
    }
    
    string op1 = instruction[1]; // destination register e.g "R0"
    string op2 = instruction[2]; // source e.g "10", "R1", "[R1]"
    
    if (isImmediate(op2)){
        int value = stoi(op2);
        // DIV - leave as ???  until lecturer clarifies!
// TODO: HOLD - spec ambiguous on R2/R1 or R1/R2

    }
    else if (isRegister(op2)){

    }
    else{
        cerr << "Invalid DIV operand" << endl;
        exit(1);
    }
    }
    void executeLOAD(vector<string> instruction){
    // 1. check operand count
    if (instruction.size() != 3){
        cerr << "LOAD expects 2 operands" << endl;
        exit(1);
    }

    
    string op1 = instruction[1]; // destination register e.g "R1"
    string op2 = instruction[2]; // always bracketed! e.g "[20]" or "[R2]"
    
    // op2 must always be bracketed for LOAD!!
    if (!isBracketed(op2)){
        cerr << "LOAD expects bracketed source!" << endl;
        exit(1);
    }
    
    string inner = stripBrackets(op2); // "[20]"→"20"  "[R2]"→"R2"
    
    if (isImmediate(inner)){
        // LOAD R1, [20]
        // inner = "20" → direct memory address
        // TODO: cpu.setRegister(op1, cpu.getMemory(stoi(inner)))
    }
    else if (isRegister(inner)){
        // LOAD R1, [R2]
        // inner = "R2" → get address from R2 first
        // TODO: cpu.setRegister(op1, cpu.getMemory(cpu.getRegister(inner)))
    }
    else{
        cerr << "Invalid LOAD operand!" << endl;
        exit(1);
    }
    }
    void executeSTORE(vector<string> instruction){
    if (instruction.size() != 3){
        cerr << "STORE expects 2 operands" << endl;
        exit(1);
    }
    
    string op1 = instruction[1];
    string op2 = instruction[2];
    
    if (isRegister(op1) && isImmediate(op2)){
        // STORE R1, 43
        // TODO: cpu.setMemory(stoi(op2), cpu.getRegister(op1))
    }
    else if (isRegister(op1) && isBracketed(op2)){
        // STORE R1, [R2]
        string inner = stripBrackets(op2); // "R2"
        // TODO: cpu.setMemory(cpu.getRegister(inner), cpu.getRegister(op1))
    }
    else if (isImmediate(op1) && isRegister(op2)){
        // STORE 20, R2
        // TODO: cpu.setMemory(stoi(op1), cpu.getRegister(op2))
    }
    else{
        cerr << "Invalid STORE operand!" << endl;
        exit(1);
    }
}


public:
    void load(string filename){
        program = parser.FileOpening(filename);
    }
    
    void run(){
    for (int i = 0; i < program.size(); i++){
        executeInstruction(program[i]);
        }
    }

    void executeInstruction(vector<string> instruction){
        string opcode = instruction[0];
        if (opcode == "INPUT") { }
        else if (opcode == "DISPLAY") { }
        else if (opcode == "MOV") { executeMOV(instruction); }  
        else if (opcode == "ADD") { executeADD(instruction); }
        else if (opcode == "SUB") { executeSUB(instruction);}
        else if (opcode == "MUL") { executeMUL(instruction);}
        else if (opcode == "DIV") { executeDIV(instruction);}
        else if (opcode == "INC") { }
        else if (opcode == "DEC") { }
        else if (opcode == "ROL") { }
        else if (opcode == "ROR") { }
        else if (opcode == "SHL") { }
        else if (opcode == "SHR") { }
        else if (opcode == "LOAD") { executeLOAD(instruction); }
        else if (opcode == "STORE") { executeSTORE(instruction);}
        else if (opcode == "RESET") { }
        else if (opcode == "PUSH") { }
        else if (opcode == "POP") { }
        else {cerr << "Unknown instruction: " << opcode << endl; }
    }

    void dump(){
        int registers[8] = {0, 11, 0, 44, 0, 0, 0, 0};
        int pc = 6;
        int memory[64] = {0}; // all zeros for now
        memory[20] = 44;

        cout << "#Begin#" << endl;
        cout << "#Registers#"; 
        for(int i=0; i<8; i++){
            cout << setfill('0') << setw(4) << registers[i] << "#";
        }
        cout << endl;

        int OF = 0, UF = 0, CF = 1, ZF = 1; //temp vals

        cout << "#Flags#"
             << "OF#" << OF << "#"
             << "UF#" << UF << "#"
             << "CF#" << CF << "#"
             << "ZF#" << ZF << "#"
             << endl;
        
        cout << "#PC#" << setfill('0') << setw(4) << pc << "#" << endl;

        cout << "#Memory#" << endl;
        for(int i = 0; i < 8; i++){        // 8 rows
            cout << "#";
            for(int j = 0; j < 8; j++){    // 8 values per row
                cout << setfill('0') << setw(4) << memory[i*8+j] << "#";
            }
            cout << endl;
        }
        cout << "#End#" << endl;
    }
};

// exception


int main(){
    Runner runner;
    runner.load("assembly.asm");
    runner.run();
    runner.dump(); 
    
    return 0;
}