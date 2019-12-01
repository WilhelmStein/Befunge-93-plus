#pragma once

#include <string>
#include <stack>

#define TORUS_X_SIZE 25
#define TORUS_Y_SIZE 80

class Interpreter
{   
    union StackVal
    {
        struct Cell
        {
            signed long int a;
            signed long int b;

            Cell(signed long int _a, signed long int _b): a(_a), b(_b) {}

        } *cell;

        signed long int value;
        
        StackVal() {}

        StackVal(signed long int v): value(v) {} 

        StackVal(signed long int a, signed long int b): cell(new Cell(a, b)) {}

        StackVal& operator=(const StackVal& b) 
        { 
            cell = b.cell;
            value = b.value;
            return *this;
        }

        operator signed long int ()
        {
            return value;
        }
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