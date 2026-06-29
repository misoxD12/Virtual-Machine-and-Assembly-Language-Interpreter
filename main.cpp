#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

//Class: VMException
//Purpose: Custom exception class used to handle virtual machine errors by storing an error message, displaying it, and terminating the program. 
//Writer: Lee Chung Sun
class VMException { 
    private:
        string message;
    public:
        VMException(string msg) { 
            message = msg;
            cout << "Error found. Terminating program." << endl;
            cout << message << endl;
            exit(1);
        }
        virtual ~VMException() {}
        string getMessage() const { return message; }
};

//Class: EmptyStackException
//Purpose: Specific exception class for handling attempts to pop from an empty stack.
//Writer: Lee Chung Sun
class EmptyStackException : public VMException {
public:
    EmptyStackException() 
    : VMException("CRITICAL ERROR: Attempted to pop from an empty stack.") {}
};

//Class: InvalidMemoryException
//Purpose: Specialized exception class used to handle invalid memory access errors by providing the problematic memory address.
//Writer: Lee Chung Sun
class InvalidMemoryException : public VMException {
public:
    InvalidMemoryException(int address) 
        : VMException("MEMORY ERROR: Invalid memory access at address " + to_string(address)) {}
};

//Class: IndexOutOfBoundsException
//Purpose: Specialized exception class used to handle invalid vector index access by providing the out-of-range index value.
//Writer: Lee Chung Sun
class IndexOutOfBoundsException : public VMException {
public:
    IndexOutOfBoundsException(int index) 
        : VMException("VECTOR ERROR: Index " + to_string(index) + " is out of bounds.") {}
};

//Class: InvalidRegisterException
//Purpose: Specialized exception class used to handle invalid register access by providing the invalid register index value.
//Writer: Lee Chung Sun
class InvalidRegisterException : public VMException {
public:
    InvalidRegisterException(int index) 
        : VMException("REGISTER ERROR: Register index " + to_string(index) + " is outside valid range (0-7).") {}
};
//Class: InvalidFlagException
//Purpose: Specialized exception class used to handle invalid flag access by providing the unknown flag name.
//Writer: Lee Chung Sun
class InvalidFlagException : public VMException {
public:
    InvalidFlagException(string flag) 
        : VMException("FLAG ERROR: Unknown flag name '" + flag + "'. Valid flags are OF, UF, CF, ZF.") {}
};
//Class: MalformedOperandException
//Purpose: Specialized exception class used to handle malformed or unexpected operand text by providing the problematic operand text.
//Writer: Lee Chung Sun
class MalformedOperandException : public VMException {
public:
    MalformedOperandException(string text) 
        : VMException("SYNTAX ERROR: Malformed or unexpected operand text: '" + text + "'.") {}
};

//Class: DivideByZeroException
//Purpose: Specialized exception class used to handle division by zero errors in mathematical operations.
//Writer: Lee Chung Sun
class DivideByZeroException : public VMException {
public:
    DivideByZeroException() : VMException("MATH ERROR: Division by zero is not allowed.") {}
};
//Class: UnknownInstructionException
//Purpose: Specialized exception class used to handle unknown or typo'd instructions by providing the problematic instruction text.
//Writer: Lee Chung Sun
class UnknownInstructionException : public VMException {
public:
    UnknownInstructionException(string inst) 
        : VMException("PARSER ERROR: Unknown or typo'd instruction '" + inst + "'.") {}
};

//Class: MultipleInstructionsException
//Purpose: Specialized exception class used to handle cases where multiple instructions or extra text are found on a single line of code by providing the line number.
//Writer: Lee Chung Sun
class MultipleInstructionsException : public VMException {
public:
    MultipleInstructionsException(int line) 
        : VMException("SYNTAX ERROR: Multiple instructions or extra text found on line " + to_string(line) + ".") {}
};
//Class: InvalidShiftCountException
//Purpose: Specialized exception class used to handle invalid shift or rotate counts in shift/rotate operations by providing the invalid count value.
//Writer: Lee Chung Sun
class InvalidShiftCountException : public VMException {
public:
    InvalidShiftCountException(int count) 
        : VMException("SHIFT ERROR: Shift/rotate count " + to_string(count) + " is invalid.") {}
};

//Class: InvalidInputException
//Purpose: Specialized exception class used to handle invalid user input for integer values by providing the problematic input text.
//Writer: Lee Chung Sun
class InvalidInputException : public VMException {
public:
    InvalidInputException(string text) 
        : VMException("INPUT ERROR: '" + text + "' is not a valid integer.") {}
};

//Class: FileIOException
//Purpose: Specialized exception class used to handle file input/output errors by providing the filename that could not be opened.
//Writer: Lee Chung Sun
class FileIOException : public VMException {
public:
    FileIOException(string filename) 
        : VMException("FILE ERROR: Unable to open file: " + filename) {}
};

//Class: InvalidOperandLogicException
//Purpose: Specialized exception class used to handle invalid operand combinations for specific instructions by providing the instruction name.
//Writer: Lee Chung Sun
class InvalidOperandLogicException : public VMException {
public:
    InvalidOperandLogicException(string instruction) 
        : VMException("EXECUTION ERROR: Invalid operand combination for " + instruction + " instruction.") {}
};

//data structure

//Class: CustomVector
//Purpose: CustomVector is a dynamic array implementation that manages elements using manually allocated memory, supporting insertion, removal, indexing, and automatic resizing.
//Writer: Lee Chung Sun
template <typename T>
class CustomVector {
private:
    T* data;           // Dynamic array to hold the elements.
    int currentSize;   // Tracks the number of actual items.
    int capacity;      // Tracks the total allocated memory size.

    // Helper function to double the array size when it gets full.
    void expand() {
int newCapacity = 0;
        if (capacity == 0) {
            newCapacity = 2;
        } else {
            newCapacity = capacity * 2;
        }        
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

//Class: CustomQueue
//Purpose: CustomQueue is a circular queue implementation that manages elements using dynamic memory, supporting FIFO operations such as enqueue and dequeue with automatic expansion.
//Writer: Lee Chung Sun
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
        int newCapacity = 0;
        if (oldCapacity == 0) {
            newCapacity = 2; // Start with a capacity of 2 if it was empty
        } else {
            newCapacity = oldCapacity * 2; // Otherwise, double the current capacity
        }
        T* newData = new T[newCapacity];
        for (int i = 0; i < count; i++) {
            // Guarded: Uses oldCapacity to safely unwrap, avoiding modulo by 0
        int safeIndex = 0;

        if (oldCapacity > 0) {
            // Calculate the index wrapping around the circular array
            safeIndex = (frontIndex + i) % oldCapacity;
        } else {
            // Default to 0 if the array has no capacity yet
            safeIndex = 0;
        }            
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

//Class: CustomStack
//Purpose: CustomStack is a dynamic stack implementation that manages elements using manually allocated memory, supporting LIFO operations such as push and pop with automatic resizing.
//Writer: Lee Chung Sun
template <typename T>
class CustomStack {
private:
    T* data;
    int capacity;
    int currentSize;

    void expand() {
        if (capacity == 0) {
            capacity = 2; // Start with a capacity of 2 if it was empty
        } else {
            capacity = capacity * 2; // Otherwise, double the current capacity
        }  
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
 
//Class: Memory
//Purpose: Memory is a memory management class that simulates a 64-byte memory space, providing safe read and write operations with address validation.
//Writer: Janine Bong Yu Ming
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

//Class: OneOperandInstruction
//Purpose: OneOperandInstruction is a base instruction class that stores a single operand and provides a common structure for instructions requiring one operand.
//Writer: Janine Bong Yu Ming
class OneOperandInstruction : public Instructions {
protected:
    Operand op;
public:
    OneOperandInstruction(int line, Operand o) : Instructions(line) {
        op = o;
    }
    virtual ~OneOperandInstruction() {}
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
//Class: IOInstruction
//Purpose: IOInstruction is a base instruction class for input and output operations that require one operand.
//Writer: Janine Bong Yu Ming
class IOInstruction : public OneOperandInstruction {
public:
    // pass parameter to base class constructor (the one operand instruction)
    IOInstruction(int line, Operand o) : OneOperandInstruction(line, o) {}
    //destructor clean up
    virtual ~IOInstruction() {}
};
//Class: INPUTInstruction
//Purpose: INPUTInstruction is an instruction class that reads user input, validates it, stores the value into a register, and updates CPU flags.
//Writer: Janine Bong Yu Ming
class INPUTInstruction : public IOInstruction {
public:
    INPUTInstruction(int line, Operand o) : IOInstruction(line, o) {}
    
    void execute(CPU &cpu) override {
        int inputVal;
        cout << "? ";
        cin >> inputVal;  // Read input 
        
        // check for non-numeric input
        if (cin.fail()) {
            cin.clear();   //reset the fail state
            cin.ignore(1000, '\n');
            throw InvalidInputException("non-numeric input");
        }
        
        // write the input value to the specified register
        cpu.setRegister(op.getRegIndex(), static_cast<signed char>(inputVal));
        // make sure the CPU flags reflect the new value
        cpu.getFlags().updateFromResult(inputVal);
    }
};


//Class: DISPLAYInstruction
//Purpose: DISPLAYInstruction is an instruction class that outputs the value stored in a specified register to the console.
//Writer: Janine Bong Yu Ming
class DISPLAYInstruction : public IOInstruction {
public:
    DISPLAYInstruction(int line, Operand o) : IOInstruction(line, o) {}
    
    void execute(CPU &cpu) override {
        //convert the signed char to int for proper display
        cout << static_cast<int>(cpu.getRegister(op.getRegIndex())) << endl;
    }
};

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
        else{ throw InvalidOperandLogicException("MOV"); }
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
        else{ throw InvalidOperandLogicException("ADD"); }
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
        else{ throw InvalidOperandLogicException("SUB"); }
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
        else{ throw InvalidOperandLogicException("MUL"); }
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
            if (cpu.getRegister(dest) == 0){ throw DivideByZeroException(); return; }
            result = op2.getValue() / cpu.getRegister(dest);
        }
        else if (op2.getType() == Register){
            if (cpu.getRegister(dest) == 0){ throw DivideByZeroException(); return; }
            result = cpu.getRegister(op2.getRegIndex()) / cpu.getRegister(dest);
        }
        else{ throw InvalidOperandLogicException("DIV"); return; }
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
        else{ throw InvalidOperandLogicException("LOAD"); }
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
    bool isOpcode(string token){
        string opcodes[] = {"MOV","ADD","SUB","MUL","DIV","INC","DEC","ROL","ROR","SHL","SHR","LOAD","STORE","RESET","PUSH","POP","INPUT","DISPLAY"};
        for (int i = 0; i < 18; i++){
            if (token == opcodes[i]) return true;
        }
        return false;
    }
public:
    Parser(){};
    CustomVector<CustomVector<string>> FileOpening(string filename) {
        CustomVector<CustomVector<string>> FinalInstructions;
        string Lines;
        int lineNum = 0;  // add counter!!
        ifstream inputFromFile(filename);
        if (inputFromFile.fail()){
            throw FileIOException(filename); 
        }
        while(getline(inputFromFile, Lines)){
            lineNum++;  // increment each line!!
            string cleanLine = Filter.toUpperCase(Filter.RemoveComma(Lines));
            CustomVector<string> ExtractedWords = Filter.split(cleanLine);
            
            if (ExtractedWords.size() > 0){
                for (int i = 1; i < ExtractedWords.size(); i++){
                    if (isOpcode(ExtractedWords[i])){
                        throw MultipleInstructionsException(lineNum);  // pass line number!!
                    }
                }
                FinalInstructions.push_back(ExtractedWords);
            }
        }
        inputFromFile.close();
        return FinalInstructions;
    }
};

class Runner {
private:
    Parser parser;
    CustomQueue<CustomVector<string>> program;
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
                throw UnknownInstructionException(opcode);
            }
        }
        if (instr != nullptr){
            instr->execute(cpu);
            delete instr;
        }
    }
public:
    void load(string filename){
    CustomVector<CustomVector<string>> parsed = parser.FileOpening(filename);
    for (int i = 0; i < parsed.size(); i++){
        program.enqueue(parsed[i]);
    }
}
    
    void run(){
    int lineNum = 0;
    while (!program.isEmpty()){
        executeInstruction(program.dequeue(), lineNum);
        cpu.incrementPC();
        lineNum++;
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
        if (outFile.fail()){ throw FileIOException("output.txt"); return; }

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
    return 0;
}