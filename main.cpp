#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
//bugggggs
using namespace std;

// exception
class VMException {
    private:
        string message;
    public:
        VMException(string msg) { message = msg; }
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
/*
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

*/

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


class CPU;


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
        if (idx >=0 && idx <= 8){
            return registers[idx] -> getRegister();
        } else{
            cerr << "invalid register number" << endl;
            return 0;
        }
    }

    void setRegister(int idx, signed char value){
        if (idx >=0 && idx <= 8){
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

    signed char getMemory(int address) const{
        return memory.readMemory(address);
    }

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

//class OneOperandInstructions : public Instructions { janine
// Logic for Math-based instructions (INC, DEC)
class MathInstruction : public Instructions {
private:
    string opCode;
    Operand op;
public:
    MathInstruction(int line, string op, Operand o) : Instructions(line), opCode(op), op(o) {}

    void execute(CPU &cpu) override {
        int val = cpu.getRegister(op.getRegIndex());
        if (opCode == "INC") val++;
        else val--;
        
        cpu.getFlags().updateFromResult(val);
        cpu.setRegister(op.getRegIndex(), static_cast<signed char>(val));
    }
};

// Logic for Stack-based instructions (PUSH, POP)
class StackInstruction : public Instructions {
private:
    string opCode;
    Operand op;
public:
    StackInstruction(int line, string op, Operand o) : Instructions(line), opCode(op), op(o) {}

    void execute(CPU &cpu) override {
        if (opCode == "PUSH") cpu.pushValue(cpu.getRegister(op.getRegIndex()));
        else cpu.setRegister(op.getRegIndex(), cpu.popValue());
    }
};

// Logic for I/O based instructions (INPUT, DISPLAY)
class IOInstruction : public Instructions {
private:
    string opCode;
    Operand op;
public:
    IOInstruction(int line, string op, Operand o) : Instructions(line), opCode(op), op(o) {}

    void execute(CPU &cpu) override {
        if (opCode == "DISPLAY") cout << static_cast<int>(cpu.getRegister(op.getRegIndex())) << endl;
        else {
            int inputVal;
            cout << "? ";
            cin >> inputVal;
            cpu.setRegister(op.getRegIndex(), static_cast<signed char>(inputVal));
            cpu.getFlags().updateFromResult(inputVal);
        }
    }
};
//class TwoOperandIntsturctions : public Instructions { SIMRAN





//pasrser

//runner


