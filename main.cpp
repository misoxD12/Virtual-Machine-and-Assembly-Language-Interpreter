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
    
public:
    void load(string filename){
        program = parser.FileOpening(filename);
    }
    
    void run(){
        for (int i = 0; i < program.size(); i++){
            vector<string> instruction = program[i];
            string opcode = instruction[0]; // first token = opcode
            
            if (opcode == "INPUT") { }
            else if (opcode == "DISPLAY") { }
            else if (opcode == "MOV") { }
            else if (opcode == "ADD") { }
            else if (opcode == "SUB") { }
            else if (opcode == "MUL") { }
            else if (opcode == "DIV") { }
            else if (opcode == "INC") { }
            else if (opcode == "DEC") { }
            else if (opcode == "ROL") { }
            else if (opcode == "ROR") { }
            else if (opcode == "SHL") { }
            else if (opcode == "SHR") { }
            else if (opcode == "LOAD") { }
            else if (opcode == "STORE") { }
            else if (opcode == "RESET") { }
            else if (opcode == "PUSH") { }
            else if (opcode == "POP") { }
            else {cerr << "Unknown instruction: " << opcode << endl; }
        }
    }

    void dump(){
        int registers[8] = {0, 11, 0, 44, 0, 0, 0, 0};
        int pc = 6;
        int memory[64] = {0}; // all zeros for now
        memory[20] = 44;

        cout << "#Begin#" << endl;
        cout << "#Registers"; 
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