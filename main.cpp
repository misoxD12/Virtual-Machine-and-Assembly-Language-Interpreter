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
    int8_t value;

public:
    Register() {
        value = 0;
    }

    int8_t getRegister() {
        return value;
    }

    void setRegister(int8_t newValue) {
        value = newValue;
    }
};

class GeneralRegister : public Register {
    int index;

public:
    GeneralRegister(int idx) {
        index = idx;
    }

    int getIndex() {
        return index;
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