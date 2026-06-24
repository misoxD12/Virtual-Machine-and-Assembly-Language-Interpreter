#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
//#include <cstdint>

using namespace std;


//data structure

//registers
class Register {
protected:
    int8_t value; //num store in this reg
public:
    Register() { //constructor start reg at 0
        value = 0;
    }

    int8_t getRegister() const { //get current value hold by the reg
        return value;
    }

    void setRegister(int8_t newValue) { //set new value for the reg
        value = newValue;
    }
};

//represent r0 - r7
class GeneralRegister : public Register {
    int index;
public:
    GeneralRegister(int idx) { //constructor assign id  (0-7)
        if (idx >= 0 && idx < 8){
            index = idx;
        } else {
            cerr << "Invalid register number" << endl;
            index = -1;
        }
    }

    int getIndex() { //get which register 
        return index;
    }
};

//4 Flags
class FlagRegister {
        bool CarryFlag, ZeroFlag, UnderflowFlag, OverflowFlag;
public:
    FlagRegister(){ //constructor init false
        CarryFlag = ZeroFlag = UnderflowFlag = OverflowFlag = false;
    }
    //getter and setter for flags
    bool getCarry() const {
        return CarryFlag;
    }
    bool getZero() const {
        return ZeroFlag;
    }
    bool getUnderflow() const {
        return UnderflowFlag;
    }
    bool getOverflow() const {
        return OverflowFlag;
    }

    void setCarry(bool value) {
        CarryFlag = value;
    }
    void setZero(bool value) {
        ZeroFlag = value;
    }
    void setUnderflow(bool value) {
        UnderflowFlag = value;
    }
    void setOverflow(bool value) {
        OverflowFlag = value;
    }

    void resetByName(string flagName) {
        if (flagName == "CF") {
            CarryFlag = false;
        } else if (flagName == "ZF") {
            ZeroFlag = false;
        } else if (flagName == "UF") {
            UnderflowFlag = false;
        } else if (flagName == "OF") {
            OverflowFlag = false;
        } else {
            cerr << "Unknown flag name" << endl;
        }
    }

    void updateFromResult(int result) {
        if (result > 127) {
            OverflowFlag = true;
        } else {
            OverflowFlag = false;
        }

        if (result < -128) {
            UnderflowFlag = true;
        } else {
            UnderflowFlag = false;
        }

        if (result == 0) {
            ZeroFlag = true;
        } else {
            ZeroFlag = false;
        }

        if (result > 255 || result < 0) {
        CarryFlag = true;
        } else {
        CarryFlag = false;
    }
};

int result = R[destination] + R[source];
flags.updateFromResult(result);

//memory

//operand??

//instructions

//cpu

//pasrser

//runner

// exception




int main(){
    cout<<"";
    return 0;
    
}