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
        string message; // Stores the error message describing the exception.
    public:
        VMException(string msg) { 
            message = msg; // Assigns the provided error message to the exception object.
            cout << "Error found. Terminating program." << endl;  // Displays a general error notification.
            cout << message << endl; // Displays the specific error message.
            exit(1); // Stops program execution after an error occurs.
        }
        virtual ~VMException() {} // Virtual destructor for safe cleanup of derived exception classes.
        string getMessage() const { 
            return message;  // Returns the stored error message.
        }
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

//Data structure
//Class: CustomVector
//Purpose: CustomVector is a dynamic array implementation that manages elements using manually allocated memory, supporting insertion, removal, indexing, and automatic resizing.
//Writer: Lee Chung Sun
template <typename T>
class CustomVector {
private:
    T* data;           // Dynamic array to hold the elements.
    int currentSize;   // Stores the current number of elements in the vector.
    int capacity;      // Stores the total allocated memory capacity of the vector.

    // Expands the dynamic array size by creating a larger array and copying existing elements.
    void expand() {
int newCapacity = 0;
        if (capacity == 0) {
            newCapacity = 2; // Set initial capacity if the array is empty.
        } else {
            newCapacity = capacity * 2; // Double the current capacity when full.
        }        
        T* newData = new T[newCapacity]; // Allocate a new larger memory block.
        for (int i = 0; i < currentSize; i++) {
            newData[i] = data[i]; // Copy existing elements into the new array.
        }
        delete[] data; // Release old memory to prevent memory leaks.
        data = newData;// Update pointer to the new array.
        capacity = newCapacity; // Update the new capacity value.
    }

public:
    CustomVector() {
        capacity = 10; // Set default starting capacity.
        currentSize = 0; // Initialize vector as empty.
        data = new T[capacity]; // Allocate memory for the elements.
    }

    ~CustomVector() {
        delete[] data;  // Free allocated memory when vector is destroyed.
    }

     // Copy constructor that creates a deep copy of another CustomVector object.
    CustomVector(const CustomVector& other) {
        capacity = other.capacity; // Copy capacity value.
        currentSize = other.currentSize;  // Copy number of elements.
        data = new T[capacity]; // Allocate new memory.
        for (int i = 0; i < currentSize; i++) {
            data[i] = other.data[i];  // Copy each element.
        }
    }

     // Assignment operator that replaces current data with a deep copy of another vector.
    CustomVector& operator=(const CustomVector& other) {
        if (this != &other) {   // Prevent self-assignment.
            delete[] data;  // Free existing memory.
            capacity = other.capacity; // Copy capacity.
            currentSize = other.currentSize; //Copy Size.
            data = new T[capacity]; // Allocate new memory.
            for (int i = 0; i < currentSize; i++) {
                data[i] = other.data[i]; // Copy elements.
            }
        }
        return *this;  // Return the updated object.
    }

    // Adds a new element to the end of the vector.
    void push_back(const T& value) {
        if (currentSize == capacity) {
            expand(); // Resize if capacity is reached.
        }
        data[currentSize] = value; // Store new element.
        currentSize++;  // Increase element count.
    }

    // Removes the last element from the vector
    void pop_back() {
        if (currentSize > 0) {
            currentSize--; // Reduce element count.
        } else {
            throw VMException("VECTOR ERROR: Attempted to pop_back from an empty vector.");
        }
    }

    // Removes an item and shifts remaining items left to prevent gaps in memory.
    void erase(int index) {
        if (index < 0 || index >= currentSize) {
            throw IndexOutOfBoundsException(index); // Prevent invalid index access.
        }
        // Shift elements left to fill the gap created by the removed item.
        for (int i = index; i < currentSize - 1; i++) {
            data[i] = data[i + 1]; // Shift elements to fill removed position.
        }
        currentSize--;
    }

    // Returns an element at the specified index with bounds checking.
    T get(int index) const {
        if (index < 0 || index >= currentSize) {
            throw IndexOutOfBoundsException(index);  // Prevent invalid access.
        }
        return data[index]; // Return requested element.
    }

    // Safely gets an item with bounds checking
    T at(int index) const {
        if (index < 0 || index >= currentSize) {
            throw IndexOutOfBoundsException(index); // Throw exception for invalid index.
        } 
        return data[index]; // Return element value.
    }

    // Overloaded [] operator for standard array-like access (Read/Write)
    T &operator[](int index) {
        return data[index]; // Return reference to element.
    }

    // Overloaded [] operator for standard array-like access (Read-Only)
    const T &operator[](int index) const {
        return data[index]; // Return constant reference to element.
    }

    // Returns the current number of stored elements.
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
        newData[i] = data[safeIndex]; // Copy elements to the new array in order.
        }
        
        delete[] data; // Free the old memory to prevent leaks.
        data = newData; // Update the data pointer to the new array.
        frontIndex = 0;
        rearIndex = count - 1;
        capacity = newCapacity; // Update the capacity to the new size.
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
        capacity = other.capacity; // Copy the capacity from the other queue
        frontIndex = other.frontIndex; // Copy the front index from the other queue
        rearIndex = other.rearIndex; // Copy the rear index from the other queue
        count = other.count; // Copy the count of elements from the other queue
        data = new T[capacity]; // Allocate new memory for the data array
        for (int i = 0; i < capacity; i++) {
            data[i] = other.data[i]; // Copy each element from the other queue's data array
        }
    }

    // Assignment Operator
    CustomQueue& operator=(const CustomQueue& other) {
        if (this != &other) {
            delete[] data; // Free existing memory to prevent leaks
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
        rearIndex = (rearIndex + 1) % capacity; // Move rear index forward in a circular manner
        data[rearIndex] = value; // Store the new value at the rear index
        count++;
    }

    T dequeue() {
        if (isEmpty()) {
            throw VMException("QUEUE ERROR: Attempted to dequeue from an empty queue.");
        }
        T value = data[frontIndex];
        // Modulo arithmetic wraps the front index back to 0 if it reaches the end.
        frontIndex = (frontIndex + 1) % capacity; // Move front index forward in a circular manner
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
            newData[i] = data[i]; // Copy existing elements into the new array.
        }
        delete[] data; 
        data = newData; // Update pointer to the new array.
    }

public:
    // Constructor initializes the stack with a default capacity of 8 elements.
    CustomStack() {
        capacity = 8; // Virtual machine stack is 8 elements.
        currentSize = 0;
        data = new T[capacity]; // Allocate memory for the stack elements.
    }

    ~CustomStack() { delete[] data; }
    // Copy constructor for deep copying another CustomStack object.
    CustomStack(const CustomStack& other) {
        capacity = other.capacity; 
        currentSize = other.currentSize;
        data = new T[capacity];
        for (int i = 0; i < currentSize; i++) {
            data[i] = other.data[i];
        }
    }
    // Assignment operator for deep copying another CustomStack object.
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
    // Adds a new element to the top of the stack, expanding if necessary.
    void push(const T& value) {
        if (currentSize == capacity) {
            expand();
        }
        // Adds item to the top of the stack.
        data[currentSize] = value;
        currentSize++;
    }
    // Removes and returns the top element of the stack, throwing an exception if the stack is empty.
    T pop() {
        if (isEmpty()) {
            // Crash/stop safely on empty pop.
            throw EmptyStackException(); 
        }
        currentSize--;
        return data[currentSize];
    }
    // Returns the current number of elements in the stack.
    bool isEmpty() const { return currentSize == 0; }
    // Returns true if the stack is full, false otherwise.
    bool isFull() const { return currentSize == capacity; }
};

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

//Class: Memory
//Purpose: Memory is a memory management class that simulates a 64-byte memory space, providing safe read and write operations with address validation.
//Writer: Janine Bong Yu Ming
class Memory {
private:
        // 1-dimensional array of 64 signed bytes
    signed char data[64];

public:
    // Constructor initializes all memory locations to zero.
    Memory(){
        for (int i = 0; i < 64; ++i) {
            data[i] = 0;
        }
    }
    // Destructor is virtual to allow for proper cleanup in derived classes.
    virtual ~Memory() {}
    // Reads a byte from the specified memory address, throwing an exception if the address is out of bounds.
    signed char readMemory(int address) const{
        if (address >= 0 && address < 64) {
            return data[address];
        } else {
            throw InvalidMemoryException(address);    
        }
    }

    // Writes a byte to the specified memory address, throwing an exception if the address is out of bounds.
    void writeMemory(int address, signed char value){
        if (address >= 0 && address < 64) {
            this->data[address] = value;
        } else {
            throw InvalidMemoryException(address); 
        }
    }

};

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
    // Execute method reads an integer from the user, validates it, and stores it in the specified register.
    void execute(CPU &cpu) override {
        int inputVal;
        cout << "? ";
        cin >> inputVal;  // Read input 
        
        // check for non-numeric input
        if (cin.fail()) {
            cin.clear();   //reset the fail state
            cin.ignore(1000, '\n'); //discard invalid input
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

        string flagLine = "#Flags#OF#" + to_string(cpu.getFlags().getOverflow()) + "#UF#" + to_string(cpu.getFlags().getUnderflow()) +
                        "#CF#" + to_string(cpu.getFlags().getCarry()) + "#ZF#" + to_string(cpu.getFlags().getZero()) + "#";//print 4 flags
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