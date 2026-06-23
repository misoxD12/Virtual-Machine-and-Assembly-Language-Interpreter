#include <iostream>
#include <fstream>
#include <sstream> // can use this meh? 
#include <string>
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


// since lect ban array i make my own one :D 
// Custom array called as MyArray

template <typename T>
class MyArray {
private: 
    T* data; 
    int TotalContentInVector;
    int CurrentArraySize; 
public: 
    MyArray(){
        TotalContentInVector = 0;
        CurrentArraySize = 10;
        data = new T[CurrentArraySize]; 
    };
    ~MyArray() {
    delete [] data; 
    };

    //member functions for array features 


    void push_back(const T &value){
        if (TotalContentInVector == CurrentArraySize){ // if full already one
            CurrentArraySize *= 2; 
            T* newData = new T[CurrentArraySize];
            for (int i=0; i<TotalContentInVector; i++){
                newData[i] = data[i];
            }
            delete [] data;
            data = newData; 
            data[TotalContentInVector] = value; 
            TotalContentInVector++;
             
        }
        else { // if not full one 
            data[TotalContentInVector] = value; 
            TotalContentInVector++;
        } 
    }

    int size()const{ return TotalContentInVector;}

    T get(int i)const{ return data[i];}

};


class Parser {
private:
    int length = 0;
public:
    Parser() {};
     void FileOpening () {
        string Lines;
        // open file here 
        ifstream inputFromFile("assembly.asm");
        // then we see if ok or not
        if (inputFromFile.fail()){cerr << "Error in opening the file" << endl;}
        else {
            while(getline(inputFromFile, Lines)){

            }
        }
        return;

    }

};

//runner

// exception




int main(){
    Parser p;
    p.FileOpening();
    cout<<"";
    return 0;
    
}