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
        if (result > 127 || result < -128){  // CHANGED only when exceeds signed char range
            CarryFlag = true;
        } else {
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
                int regNum = inside[1] - 1 - '0'; // not CHANGED but Claude mention got bug? can u help check 
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

        op.setType(Immediate); // CHANGED 
            int num = 0;
            int startIndex = (text[0] == '-') ? 1 : 0;  // skip '-' if negative
            if (len == startIndex){
                throw MalformedOperandException(text);
            }
            for (int i = startIndex; i < len; i++){
                if (text[i] < '0' || text[i] > '9'){
                    throw MalformedOperandException(text);
                }
                num = num * 10 + (text[i] - '0');
            }
            if (startIndex == 1) num = -num;  // apply negative sign
            op.setValue(num);
            return op;
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
    
    signed char read(int address) const{
        if (address >= 0 && address < 64) {
            return data[address];
        } else {
            throw InvalidMemoryException(address); // Item #8    
        }
    }

       
    void write(int address, signed char value){
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
    CustomStack <signed char> stack; // CHANGED 

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

    // PLACEHOLDER 
    signed char getMemory(int address) const{
        return memory.read(address);    
    }
    
    void setMemory(int address, signed char value){
    memory.write(address, value);
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


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// from janine part 

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

class OneOperandInstruction : public Instructions {
protected:
    Operand op;
public:
    OneOperandInstruction(int line, Operand o) : Instructions(line) {
        op = o;
    }
    virtual ~OneOperandInstruction() {}
    virtual void execute(CPU &cpu) = 0; 
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

class INPUTInstruction : public OneOperandInstruction {
public:
    INPUTInstruction(int line, Operand o) : OneOperandInstruction(line, o) {}
    
    void execute(CPU &cpu) override {
        int inputVal;
        cout << "? ";
        cin >> inputVal;
        
        cpu.setRegister(op.getRegIndex(), static_cast<signed char>(inputVal));
        cpu.getFlags().updateFromResult(inputVal);
    }
};

class DISPLAYInstruction : public OneOperandInstruction {
public:
    DISPLAYInstruction(int line, Operand o) : OneOperandInstruction(line, o) {}
    
    void execute(CPU &cpu) override {
        cout << static_cast<int>(cpu.getRegister(op.getRegIndex())) << endl;
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
            cpu.setRegister(dest, cpu.getMemory(cpu.getRegister(op2.getRegIndex())));
        }
        else if (op2.getType() == directMem){
            cpu.setRegister(dest, cpu.getMemory(op2.getValue()));
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
        int dest = op1.getRegIndex();
        int result;
        if (op2.getType() == Immediate){
            if (cpu.getRegister(dest) == 0){ cerr << "Division by zero!" << endl; return; }
            result = op2.getValue() / cpu.getRegister(dest);
        }
        else if (op2.getType() == Register){
            if (cpu.getRegister(dest) == 0){ cerr << "Division by zero!" << endl; return; }
            result = cpu.getRegister(op2.getRegIndex()) / cpu.getRegister(dest);
        }
        else{ cerr << "Invalid DIV operand!" << endl; return; }
        cpu.setRegister(dest, (signed char)result);
        cpu.getFlags().updateFromResult(result);
    }
};

class LOADInstruction : public TwoOperandInstruction {
public:
    LOADInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        int dest = op1.getRegIndex();
        if (op2.getType() == indirectMem){
            // LOAD R1, [R2]
            cpu.setRegister(dest, cpu.getMemory(cpu.getRegister(op2.getRegIndex())));
        }
        else if (op2.getType() == directMem){
            // LOAD R1, [20]
            cpu.setRegister(dest, cpu.getMemory(op2.getValue()));
        }
        else{ cerr << "Invalid LOAD operand!" << endl; }
    }
};

class STOREInstruction : public TwoOperandInstruction {
public:
    STOREInstruction(int line, Operand o1, Operand o2) : TwoOperandInstruction(line, o1, o2){}
    void execute(CPU &cpu){
        if (op1.getType() == Register && op2.getType() == Immediate){
            // STORE R1, 43 → store R1's value into address 43
            cpu.setMemory(op2.getValue(), cpu.getRegister(op1.getRegIndex()));
        }
        else if (op1.getType() == Register && op2.getType() == indirectMem){
            // STORE R1, [R2]
            cpu.setMemory(cpu.getRegister(op2.getRegIndex()), cpu.getRegister(op1.getRegIndex()));
        }
        else if (op1.getType() == Immediate && op2.getType() == Register){
            // STORE 20, R3 → store R3's value into address 20
            cpu.setMemory(op1.getValue(), cpu.getRegister(op2.getRegIndex()));
        }
        else{ cerr << "Invalid STORE operand!" << endl; }
    }
};

//-----------------------------------------------------------------------------------------------------------------------------------------------
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


class FilterFromFile {
public:
    string RemoveComma(string line){
        for (int i=0; i<line.length(); i++){
            if (line[i] == ',') {line[i] = ' ';}
        } return line;
    }
    
    CustomVector<string> split(string FilteredLine) {
        CustomVector<string> words;
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
    
    CustomVector<CustomVector<string>> FileOpening(string filename) {
        CustomVector<CustomVector<string>> FinalInstructions;
        
        string Lines;
        ifstream inputFromFile(filename);
        if (inputFromFile.fail()){
            cerr << "Error in opening the file" << endl; 
            return FinalInstructions;
        } else {
            while(getline(inputFromFile, Lines)){
                string cleanLine = Filter.toUpperCase(Filter.RemoveComma(Lines));
                CustomVector<string> ExtractedWords = Filter.split(cleanLine);
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
    CustomVector<CustomVector<string>> program;
    CustomQueue<string> executionHistory;
    CPU cpu;

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

    void executeInstruction(CustomVector<string> instruction, int lineNum){
        string opcode = instruction[0];
        Instructions* instr = nullptr;
        Operand operandParser;

        bool isOneOperand = (opcode=="RESET"||opcode=="INC"||opcode=="DEC"||opcode=="PUSH"||opcode=="POP"||opcode=="INPUT"||opcode=="DISPLAY");
        if (isOneOperand){
            instr = createOneOperand(opcode, lineNum, instruction);
        } else {
            Operand op1, op2;
            if (instruction.size() > 1) op1 = operandParser.readOperand(instruction[1]);
            if (instruction.size() > 2) op2 = operandParser.readOperand(instruction[2]);
            instr = createTwoOperand(opcode, lineNum, op1, op2);
            if (instr == nullptr){
                cerr << "Unknown instruction: " << opcode << endl;
                exit(1);
            }
        }
        if (instr != nullptr){
            instr->execute(cpu);
            delete instr;
        }
        executionHistory.enqueue("Line " + to_string(lineNum+1) + ": executed " + opcode);
    }
public:
    void load(string filename){
        program = parser.FileOpening(filename);
    }

    void printHistory(){
        cout << endl << "<>-- Execution History --<>" << endl;
        while (!executionHistory.isEmpty()){
            cout << executionHistory.dequeue() << endl;
        }
    }
    
    void run(){
        for (int i = 0; i < program.size(); i++){
            executeInstruction(program[i], i);
            cpu.incrementPC();

        }
    }

    void writeLine(ofstream &outFile, string text){
        cout << text << endl;
        outFile << text << endl;
    }

    string formatVal(int val, int width){
        string digits = to_string(abs(val));
        while((int)digits.length() < width) digits = "0" + digits;
        return (val < 0 ? "-" : "") + digits;
    }

    void dump(){
        ofstream outFile("output.txt");
        if (outFile.fail()){ cerr << "Error opening output file!" << endl; return; }

        writeLine(outFile, "#Begin#");

        // registers
        string regLine = "#Registers#";
        for(int i = 0; i < 8; i++){
            int val = (int)cpu.getRegister(i);
            regLine += formatVal(val, val < 0 ? 3 : 4) + "#";
        }
        writeLine(outFile, regLine);

        // flags
        string flagLine = "#Flags#OF#" + to_string(cpu.getFlags().getOverflow()) +
                        "#UF#" + to_string(cpu.getFlags().getUnderflow()) +
                        "#CF#" + to_string(cpu.getFlags().getCarry()) +
                        "#ZF#" + to_string(cpu.getFlags().getZero()) + "#";
        writeLine(outFile, flagLine);

        // PC
        writeLine(outFile, "#PC#" + formatVal(cpu.getPC(), 4) + "#");

        // memory
        writeLine(outFile, "#Memory#");
        for(int i = 0; i < 8; i++){
            string memLine = "#";
            for(int j = 0; j < 8; j++){
                int val = (int)cpu.getMemory(i*8+j);
                memLine += formatVal(val, val < 0 ? 3 : 4) + "#";
            }
            writeLine(outFile, memLine);
        }

        writeLine(outFile, "#End#");
        outFile.close();
    }
};

int main(){
    Runner runner;
    runner.load("assembly.asm");
    runner.run();
    runner.dump(); 
    runner.printHistory();
    return 0;
}