#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#define TORUS_X_SIZE 25
#define TORUS_Y_SIZE 80

class Interpreter
{   
    struct StackVal
    {
        signed long int value : 62;
        bool marked : 1;
        bool is_ptr : 1;
        
        StackVal(): value(0), marked(false), is_ptr(false) {}

        StackVal(signed long int v): value(v), marked(false), is_ptr(false) {} 

        StackVal(signed long int a, signed long int b): value((signed long int) new Cell(a, b)), marked(false), is_ptr(false) {}

        StackVal& operator=(const StackVal& b) 
        { 
            value = b.value;
            marked = b.marked;
            is_ptr = b.is_ptr;

            return *this;
        }

        operator signed long int ()
        {
            return value;
        }

        static void dfs(StackVal& s)
        {
            if(s.marked)
                return;
            else
                s.marked = true;
            
            if(s.is_ptr)
            {
                dfs(((Cell*)s.value)->a);
                dfs(((Cell*)s.value)->b);
            }
            else
                return;
        }

        static void undfs(StackVal& s)
        {
            if(s.marked == false)
                return;
            else
                s.marked = false;
            
            if(s.is_ptr)
            {
                undfs(((Cell*)s.value)->a);
                undfs(((Cell*)s.value)->b);
            }
            else
                return;
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
    std::vector<Interpreter::StackVal> program_stack;
    std::unordered_set<signed long int> program_heap;
    

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

    // Garbage Collector functions
    void mark();
    void sweep();
    void unmark();

    // Misc Helper Functions
    void print_torus();

public:
    Interpreter();
    ~Interpreter();

    bool load(std::string program_path);
    int execute();
};