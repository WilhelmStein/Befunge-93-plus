#pragma once

#include <string>
#include <stack>

#define TORUS_X_SIZE 25
#define TORUS_Y_SIZE 80

class Interpreter
{

    /*enum Opcode {
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE,
        MODULO
    };*/

    char program_code[TORUS_X_SIZE][TORUS_Y_SIZE]; // Program code modelled as a 25x80 torus
    short int pcx, pcy; // Program counters for the x and y dimension of the program_code respectively
    std::stack<signed long int> program_stack;

    enum Direction {
        Up,
        Down,
        Left,
        Right
    } pc_dir;


    // Program Counter alteration functions
    void inc_xcounter();
    void red_xcounter();
    void inc_ycounter();
    void red_ycounter();
    void inc_counter();

    // Stack pop function
    signed long int pop();

    // Misc Helper Functions
    void print_torus();

public:
    Interpreter();
    ~Interpreter();

    bool load(std::string program_path);
    int execute();
};