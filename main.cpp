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

    int8_t getRegister() { //get current value hold by the reg
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
};


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