#pragma once

#include <string>
#include <stack>

#define TORUS_X_SIZE 25
#define TORUS_Y_SIZE 80

class Interpreter
{   
    struct StackVal
    {
        signed long value : 62;
        bool marked : 1;
        bool is_ptr : 1;
        
        StackVal() {}

        StackVal(signed long int v): value(v), is_ptr(false) {} 

        StackVal(signed long int a, signed long int b): value((long) new Cell(a, b)), is_ptr(true) {}

        StackVal& operator=(const StackVal& b) 
        { 
            value = b.value;
            return *this;
        }

        operator signed long int ()
        {
            return value;
        }
    };

    struct Cell
    {
        StackVal a;
        StackVal b;

        Cell(StackVal _a, StackVal _b): a(_a), b(_b) {}

    };


    char program_code[TORUS_X_SIZE][TORUS_Y_SIZE]; // Program code modelled as a 25x80 torus
    short int pcx, pcy; // Program counters for the x and y dimension of the program_code respectively
    std::stack<Interpreter::StackVal> program_stack;


    enum Direction {
        Up,
        Down,
        Left,
        Right
    } pc_dir;


    // Program Counter alteration functions
    void inc_counter();

    // Stack pop function
    StackVal pop();

    // Misc Helper Functions
    void print_torus();

public:
    Interpreter();
    ~Interpreter();

    bool load(std::string program_path);
    int execute();
};