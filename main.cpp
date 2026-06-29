#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

// exception
class VMException {
    private:
        string message;
    public:
        VMException(string msg) { 
            message = msg;
            cout << message << endl;
            exit(1);
        }
        virtual ~VMException() {}
        string getMessage() const { return message; }
};

// Specific exceptions inheriting from the base class
class EmptyStackException : public VMException {
public:
    EmptyStackException() : VMException("CRITICAL ERROR: Attempted to pop from an empty stack.") {}
};

class InvalidMemoryException : public VMException {
public:
    InvalidMemoryException(int address) 
        : VMException("MEMORY ERROR: Invalid memory access at address " + to_string(address)) {}
};

class IndexOutOfBoundsException : public VMException {
public:
    IndexOutOfBoundsException(int index) 
        : VMException("VECTOR ERROR: Index " + to_string(index) + " is out of bounds.") {}
};

class InvalidRegisterException : public VMException {
public:
    InvalidRegisterException(int index) 
        : VMException("REGISTER ERROR: Register index " + to_string(index) + " is outside valid range (0-7).") {} // Covers Table Items #1 & #2
};

class InvalidFlagException : public VMException {
public:
    InvalidFlagException(string flag) 
        : VMException("FLAG ERROR: Unknown flag name '" + flag + "'. Valid flags are CF, ZF, UF, OF.") {} // Covers Table Item #3
};

class MalformedOperandException : public VMException {
public:
    MalformedOperandException(string text) 
        : VMException("SYNTAX ERROR: Malformed or unexpected operand text: '" + text + "'.") {} // Covers Table Item #6
};

class DivideByZeroException : public VMException {
public:
    DivideByZeroException() : VMException("MATH ERROR: Division by zero is not allowed.") {} // Covers Item #7
};

class UnknownInstructionException : public VMException {
public:
    UnknownInstructionException(string inst) 
        : VMException("PARSER ERROR: Unknown or typo'd instruction '" + inst + "'.") {} // Covers Item #9
};

class MultipleInstructionsException : public VMException {
public:
    MultipleInstructionsException(int line) 
        : VMException("SYNTAX ERROR: Multiple instructions or extra text found on line " + to_string(line) + ".") {} // Covers Item #10
};

class InvalidShiftCountException : public VMException {
public:
    InvalidShiftCountException(int count) 
        : VMException("SHIFT ERROR: Shift/rotate count " + to_string(count) + " is invalid.") {}
};

class InvalidInputException : public VMException {
public:
    InvalidInputException(string text) 
        : VMException("INPUT ERROR: '" + text + "' is not a valid integer.") {}
};

class FileIOException : public VMException {
public:
    FileIOException(string filename) 
        : VMException("FILE ERROR: Unable to open file: " + filename) {}
};

class InvalidOperandLogicException : public VMException {
public:
    InvalidOperandLogicException(string instruction) 
        : VMException("EXECUTION ERROR: Invalid operand combination for " + instruction + " instruction.") {}
};

//data structure

//vector
template <typename T>
class CustomVector {
private:
    T* data;           // Dynamic array to hold the elements.
    int currentSize;   // Tracks the number of actual items.
    int capacity;      // Tracks the total allocated memory size.

    // Helper function to double the array size when it gets full.
    void expand() {
        int newCapacity = (capacity == 0) ? 2 : capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i < currentSize; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    CustomVector() {
        capacity = 10;
        currentSize = 0;
        data = new T[capacity];
    }

    ~CustomVector() {
        delete[] data; // Clean up memory to prevent leaks.
    }

    // Copy Constructor: Performs a deep copy of the underlying array
    CustomVector(const CustomVector& other) {
        capacity = other.capacity;
        currentSize = other.currentSize;
        data = new T[capacity];
        for (int i = 0; i < currentSize; i++) {
            data[i] = other.data[i];
        }
    }

    // Assignment Operator: Cleans up existing memory before copying
    CustomVector& operator=(const CustomVector& other) {
        if (this != &other) { 
            delete[] data; 
            capacity = other.capacity;
            currentSize = other.currentSize;
            data = new T[capacity];
            for (int i = 0; i < currentSize; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    void push_back(const T& value) {
        if (currentSize == capacity) {
            expand(); // Resize if capacity is reached.
        }
        data[currentSize] = value;
        currentSize++;
    }

    // Removes the last element from the vector
    void pop_back() {
        if (currentSize > 0) {
            currentSize--;
        } else {
            throw VMException("VECTOR ERROR: Attempted to pop_back from an empty vector.");
        }
    }

    // Removes an item and shifts remaining items left to prevent gaps in memory.
    void erase(int index) {
        if (index < 0 || index >= currentSize) {
            throw IndexOutOfBoundsException(index);
        }
        // Shift elements left to fill the gap created by the removed item.
        for (int i = index; i < currentSize - 1; i++) {
            data[i] = data[i + 1];
        }
        currentSize--;
    }

    T get(int index) const {
        if (index < 0 || index >= currentSize) {
            throw IndexOutOfBoundsException(index);
        }
        return data[index];
    }

    // Safely gets an item with bounds checking
    T at(int index) const {
        if (index < 0 || index >= currentSize) {
            throw IndexOutOfBoundsException(index);
        }
        return data[index];
    }

    // Overloaded [] operator for standard array-like access (Read/Write)
    T &operator[](int index) {
        return data[index];
    }

    // Overloaded [] operator for standard array-like access (Read-Only)
    const T &operator[](int index) const {
        return data[index];
    }

    int size() const { return currentSize; }
};
//Queue
template <typename T>
class CustomQueue {
private:
    T* data;
    int capacity;
    int frontIndex; // Points to the first item.
    int rearIndex;  // Points to the last item.
    int count;      // Tracks total items currently in the queue.

    // Expands the array and realigns the circular structure into a straight line.
    void expand() {
        int oldCapacity = capacity;
        int newCapacity = (oldCapacity == 0) ? 2 : oldCapacity * 2;
        T* newData = new T[newCapacity];
        
        for (int i = 0; i < count; i++) {
            // Guarded: Uses oldCapacity to safely unwrap, avoiding modulo by 0
            int safeIndex = (oldCapacity > 0) ? ((frontIndex + i) % oldCapacity) : 0;
            newData[i] = data[safeIndex];
        }
        
        delete[] data;
        data = newData;
        frontIndex = 0;
        rearIndex = count - 1;
        capacity = newCapacity;
    }

public:
    CustomQueue() {
        capacity = 10;
        data = new T[capacity];
        frontIndex = 0;
        rearIndex = -1;
        count = 0;
    }

    ~CustomQueue() { delete[] data; }

    // Copy Constructor
    CustomQueue(const CustomQueue& other) {
        capacity = other.capacity;
        frontIndex = other.frontIndex;
        rearIndex = other.rearIndex;
        count = other.count;
        data = new T[capacity];
        for (int i = 0; i < capacity; i++) {
            data[i] = other.data[i];
        }
    }

    // Assignment Operator
    CustomQueue& operator=(const CustomQueue& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            frontIndex = other.frontIndex;
            rearIndex = other.rearIndex;
            count = other.count;
            data = new T[capacity];
            for (int i = 0; i < capacity; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    void enqueue(const T& value) {
        if (count == capacity) {
            expand();
        }
        // Modulo arithmetic wraps the rear index back to 0 if it reaches the end.
        rearIndex = (rearIndex + 1) % capacity;
        data[rearIndex] = value;
        count++;
    }

    T dequeue() {
        if (isEmpty()) {
            throw VMException("QUEUE ERROR: Attempted to dequeue from an empty queue.");
        }
        T value = data[frontIndex];
        // Modulo arithmetic wraps the front index back to 0 if it reaches the end.
        frontIndex = (frontIndex + 1) % capacity;
        count--;
        return value;
    }

    bool isEmpty() const { return count == 0; }
};
//Stack
template <typename T>
class CustomStack {
private:
    T* data;
    int capacity;
    int currentSize;

    void expand() {
        capacity = (capacity == 0) ? 2 : capacity * 2;
        T* newData = new T[capacity];
        for (int i = 0; i < currentSize; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    CustomStack() {
        capacity = 8; // Virtual machine stack is 8 elements.
        currentSize = 0;
        data = new T[capacity];
    }

    ~CustomStack() { delete[] data; }

    CustomStack(const CustomStack& other) {
        capacity = other.capacity;
        currentSize = other.currentSize;
        data = new T[capacity];
        for (int i = 0; i < currentSize; i++) {
            data[i] = other.data[i];
        }
    }

    CustomStack& operator=(const CustomStack& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            currentSize = other.currentSize;
            data = new T[capacity];
            for (int i = 0; i < currentSize; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    void push(const T& value) {
        if (currentSize == capacity) {
            expand();
        }
        // Adds item to the top of the stack.
        data[currentSize] = value;
        currentSize++;
    }

    T pop() {
        if (isEmpty()) {
            // Assignment constraint: Crash/stop safely on empty pop.
            throw EmptyStackException(); 
        }
        currentSize--;
        return data[currentSize];
    }

    bool isEmpty() const { return currentSize == 0; }

    bool isFull() const { return currentSize == capacity; }
};


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
            throw InvalidRegisterException(idx);
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
            throw InvalidFlagException(flagName);
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
        } else {
            CarryFlag = false;
        }
    }
};


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

    //handles the case where text has brackets
    Operand readBracketOperand(string text, int len){
        Operand op;
        //store the content in the brackets
        string inside = "";
        for (int i = 1; i < len-1; i++){
            inside = inside + text [i];
        }

        if (isValidRegister(inside)){
            op.setType(indirectMem);
            int regNum = inside[1] - '0';
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
    Operand readImmediateOperand(string text, int len){
        Operand op;
        op.setType(Immediate);

        int startIndex = 0;
        bool isNegative = false;
        if (text[0] == '-'){
            startIndex = 1;
            isNegative = true;
        }

        int num = 0;
        for (int i = startIndex; i < len; i++){
            num = num * 10 + (text[i] - '0');
        }

        if(isNegative){
            num = -num;
        }

        op.setValue(num);
        return op;
    }

    Operand readOperand(string text){
        int len = text.length();
        if(len == 0){
            throw MalformedOperandException("Empty Operand");
        };
        //check got brackets?
        bool hasBrackets = (text[0] == '[' && text[len - 1] == ']');

        if (hasBrackets) {
            return readBracketOperand(text, len);
        }

        if (isValidRegister(text)){
            Operand op;
            op.setType (Register);
            int regNum = text[1] - '0';
            op.setRegIndex(regNum);
            return op;
        }

        if (text[0] == 'R' || text[0] == 'r') {
            throw MalformedOperandException(text);   //looked like a register, but wasn't valid 
        }

        return readImmediateOperand(text, len);
    }
};

//memory
class Memory {
private:
        // 1-dimensional array of 64 signed bytes
    signed char data[64];

public:
       
    Memory(){
        for (int i = 0; i < 64; ++i) {
            data[i] = 0;
        }
    }
    
    virtual ~Memory() {}

    signed char readMemory(int address) const{
        if (address >= 0 && address < 64) {
            return data[address];
        } else {
            throw InvalidMemoryException(address); // Item #8    
        }
    }

       
    void writeMemory(int address, signed char value){
        if (address >= 0 && address < 64) {
            this->data[address] = value;
        } else {
            throw InvalidMemoryException(address); // Item #8    
        }
    }

};

//cpu
class CPU {
    GeneralRegister* registers[8]; //8 pointers r0-r7
    FlagRegister flags;
    int PC;
    int SI;

    Memory memory;
    CustomStack <signed char> stack; 

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
            throw InvalidRegisterException(idx);
        }
    }

    void setRegister(int idx, signed char value){
        if (idx >=0 && idx < 8){
            registers[idx] -> setRegister(value);
        } else{
            throw InvalidRegisterException(idx);
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

    // PLACEHOLDER 
    signed char getMemory(int address) const{
        return memory.readMemory(address);    
    }
    
    void setMemory(int address, signed char value){
    memory.writeMemory(address, value);
}

    // PLACEHOLDER 
    void pushValue(signed char value){
        stack.push(value);
        SI = SI + 1;
    }

    signed char popValue(){
        signed char v = stack.pop();
        SI = SI - 1;
        return v;
        
    }
        
};

//instructions
class Instructions{
protected:
    int lineNum;
public:
    Instructions(int line){ //constructor
        lineNum = line;
    }
    virtual ~Instructions(){}
    virtual void execute(CPU &cpu) = 0; //for instrcution polymorphsm

    int getLineNum() const{
        return lineNum;
    }

};

class OneOperandInstruction : public Instructions {
protected:
    Operand op;
public:
    OneOperandInstruction(int line, Operand o) : Instructions(line) {
        op = o;
    }
    virtual ~OneOperandInstruction() {}
};

class IOInstruction : public OneOperandInstruction {
public:
    IOInstruction(int line, Operand o) : OneOperandInstruction(line, o){}
    virtual ~IOInstruction(){}
};

class RESETInstruction : public Instructions {
private:
    string targetFlag;
public:
    RESETInstruction(int line, string flag) : Instructions(line), targetFlag(flag) {}
    
    void execute(CPU &cpu) override {
        // Your FlagRegister class already has this perfect helper function!
        cpu.getFlags().resetByName(targetFlag);
    }
};


class INCInstruction : public OneOperandInstruction {
public:
    INCInstruction(int line, Operand o) : OneOperandInstruction(line, o) {}
    
    void execute(CPU &cpu) override {
        int val = cpu.getRegister(op.getRegIndex());
        val++;
        
        cpu.getFlags().updateFromResult(val);
        cpu.setRegister(op.getRegIndex(), static_cast<signed char>(val));
    }
};

class DECInstruction : public OneOperandInstruction {
public:
    DECInstruction(int line, Operand o) : OneOperandInstruction(line, o) {}
    
    void execute(CPU &cpu) override {
        int val = cpu.getRegister(op.getRegIndex());
        val--;
        
        cpu.getFlags().updateFromResult(val);
        cpu.setRegister(op.getRegIndex(), static_cast<signed char>(val));
    }
};

class PUSHInstruction : public OneOperandInstruction {
public:
    PUSHInstruction(int line, Operand o) : OneOperandInstruction(line, o) {}
    
    void execute(CPU &cpu) override {
        cpu.pushValue(cpu.getRegister(op.getRegIndex()));
    }
};

class POPInstruction : public OneOperandInstruction {
public:
    POPInstruction(int line, Operand o) : OneOperandInstruction(line, o) {}
    
    void execute(CPU &cpu) override {
        cpu.setRegister(op.getRegIndex(), cpu.popValue());
    }
};

class INPUTInstruction : public IOInstruction {
public:
    INPUTInstruction(int line, Operand o) : IOInstruction(line, o) {}
    
    void execute(CPU &cpu) override {
        int inputVal;
        cout << "? ";
        cin >> inputVal;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            throw InvalidInputException("non-numeric input");
        }
        
        cpu.setRegister(op.getRegIndex(), static_cast<signed char>(inputVal));
        cpu.getFlags().updateFromResult(inputVal);
    }
};

class DISPLAYInstruction : public IOInstruction {
public:
    DISPLAYInstruction(int line, Operand o) : IOInstruction(line, o) {}
    
    void execute(CPU &cpu) override {
        cout << static_cast<int>(cpu.getRegister(op.getRegIndex())) << endl;
    }
};
//Class: TwoOperandInstruction 
// Purpose: thisclass for all instructions that take 2 operands
// Writer: [Harsimran]
class TwoOperandInstruction : public Instructions {
protected:
    Operand op1, op2;
public:
    TwoOperandInstruction(int line, Operand o1, Operand o2) : Instructions(line){
        op1 = o1;
        op2 = o2;
    }
    virtual ~TwoOperandInstruction(){}
};
// Class: ArithmeticInstruction
// Puropuse: this class groups all arithmetic instructions  which is the ADD/SUB/MUL/DIV
// Writer: [Harsimran]
class ArithmeticInstruction : public TwoOperandInstruction {
public:
    ArithmeticInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    virtual ~ArithmeticInstruction(){}
};
// class: MOVInstruction
// Purpose: this class copy the value into destination register. Supports 4 modes which are immediate, register, indirect, direct
// Writer: [Harsimran]

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
            cpu.setRegister(dest, cpu.getMemory(cpu.getRegister(op2.getRegIndex())));
        }
        else if (op2.getType() == directMem){
            cpu.setRegister(dest, cpu.getMemory(op2.getValue()));
        }
        else{ throw InvalidOperandLogicException("MOV"); }
    }
};
// Class: ADDInstruction
// Purpose: this class add op2 to op1, stores in  op1 updates flags after operation
// Writer: [Harsimran]
class ADDInstruction : public ArithmeticInstruction {
public:
    ADDInstruction(int line, Operand o1, Operand o2) : ArithmeticInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex();
        int result;
        if (op2.getType() == Immediate){
            result = cpu.getRegister(dest) + op2.getValue();
        }
        else if (op2.getType() == Register){
            result = cpu.getRegister(dest) + cpu.getRegister(op2.getRegIndex());
        }
        else{ throw InvalidOperandLogicException("ADD"); }
        cpu.setRegister(dest, (signed char)result);
        cpu.getFlags().updateFromResult(result);
    }
};

// Class: SUBInstruction 
// Purpose: this class subtract op2 to op1, stores in op1 updates flags after operation
// Writer: [Harsimran]
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
        cpu.getFlags().updateFromResult(result);
    }
};
// Class :MULInstruction
// Purpose: this class multiply op2 to op1, stores in op1 updates flags after operation
// Writer: [Harsimran]

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
        cpu.getFlags().updateFromResult(result);
    }
};
// Class: DIVInstruction
// Purpose: this class subtract op2 to op1, stores in op1 updates flags after operation
// Writer: [Harsimran]
class DIVInstruction : public ArithmeticInstruction {
public:
    DIVInstruction(int line, Operand o1, Operand o2) : ArithmeticInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex();
        int result;
        if (op2.getType() == Immediate){
            if (op2.getValue() == 0){ throw DivideByZeroException(); return; }
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
// Writer: [Harsimran]
class LOADInstruction : public TwoOperandInstruction {
public:
    LOADInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex();
        if (op2.getType() == indirectMem){
            cpu.setRegister(dest, cpu.getMemory(cpu.getRegister(op2.getRegIndex())));
        }
        else if (op2.getType() == directMem){
            cpu.setRegister(dest, cpu.getMemory(op2.getValue()));
        }
        else{ throw InvalidOperandLogicException("LOAD"); }
    }
};
//Class: STOREInstruction 
// Purpose: this class stores register value into memory and support 3 modes
// Writer: [Harsimran]

class STOREInstruction : public TwoOperandInstruction {
public:
    STOREInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        if (op1.getType() == Register && op2.getType() == Immediate){
            cpu.setMemory(op2.getValue(), cpu.getRegister(op1.getRegIndex()));
        }
        else if (op1.getType() == Register && op2.getType() == indirectMem){
            cpu.setMemory(cpu.getRegister(op2.getRegIndex()), cpu.getRegister(op1.getRegIndex()));
        }
        else if (op1.getType() == Immediate && op2.getType() == Register){
            cpu.setMemory(op1.getValue(), cpu.getRegister(op2.getRegIndex()));
        }
        else{ throw InvalidOperandLogicException("STORE"); }
    }
};

class ShiftInstruction : public TwoOperandInstruction {
protected:
    int destReg;
    int count;

    unsigned char convertUnsigned(CPU &cpu){
        return static_cast<unsigned char>(cpu.getRegister(destReg));
    }

    void storeResult(CPU &cpu, unsigned char val){
        cpu.setRegister(destReg, static_cast<signed char>(val));
    }
public:
    ShiftInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2) {
        destReg = o1.getRegIndex();
        count = o2.getValue();
        if (count < 0){
            throw InvalidShiftCountException(count);
        }
    }

    virtual ~ShiftInstruction(){
    }
};

class ROLCommand : public ShiftInstruction {
public:
    ROLCommand(int line, Operand o1, Operand o2) : ShiftInstruction(line, o1, o2) {
    }

    void execute(CPU &cpu) override {
        unsigned char val = convertUnsigned(cpu);
        int shiftCount   = count % 8;
        val = (val << shiftCount) | (val >> (8 - shiftCount));
        storeResult(cpu, val);
    }
};

class RORCommand : public ShiftInstruction {
public:
    RORCommand(int line, Operand o1, Operand o2) : ShiftInstruction(line, o1, o2) {
    }

    void execute(CPU &cpu) override {
        unsigned char val = convertUnsigned(cpu);
        int shiftCount = count % 8;
        val = (val >> shiftCount) | (val << (8 - shiftCount));
        storeResult(cpu, val);
    }
};

class SHLCommand : public ShiftInstruction {
public:
    SHLCommand(int line, Operand o1, Operand o2) : ShiftInstruction(line, o1, o2) {
    }

    void execute(CPU &cpu) override {
        unsigned char val = convertUnsigned(cpu);
        if (count >= 8) {
            val = 0;
        } else {
            val = val << count;
        }
        storeResult(cpu, val);
    }
};

class SHRCommand : public ShiftInstruction {
public:
    SHRCommand(int line, Operand o1, Operand o2) : ShiftInstruction(line, o1, o2) {
    }

    void execute(CPU &cpu) override {
        unsigned char val = convertUnsigned(cpu);
        if (count >= 8) {
            val = 0;
        } else {
            val = val >> count;
        }
        storeResult(cpu, val);
    }
};


// Class: FilterFromFile
// Purpose: this class cleans the raw lines from .asm file before parsing
// Writer: [Harsimran]
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
// Writer: [Harsimran]
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
// Writer: [Harsimran]

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

int main(){ // main part of the program that calls the runner and the functions 
    Runner runner;
    runner.load("assembly.asm");
    runner.run();
    runner.dump(); 
    return 0;
}