#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#define TORUS_X_SIZE 25
#define TORUS_Y_SIZE 80

#define STACK_SIZE (1L << 20)
#define HEAP_SIZE  (1L << 24)

class Interpreter
{   
    struct Cell;
    
    struct Value
    {
        signed long int value : 63;
        bool is_ptr : 1;
        
        Value(): value(0), is_ptr(false) {}

        Value(signed long int v): value(v), is_ptr(false) {} 

        Value(Value a, Value b, Cell*& freeList): value((signed long int)freeList), is_ptr(true)
        {
            freeList = (Cell*) freeList->a.value;
            ((Cell*)value)->a = a;
            ((Cell*)value)->b = b;
        }

        Value& operator=(const Value& b) 
        { 
            value = b.value;
            is_ptr = b.is_ptr;

            return *this;
        }

        operator signed long int ()
        {
            return value;
        }

        void dfs()
        {

            if(is_ptr && !((Cell *)value)->marked)
            {
                ((Cell *)value)->marked = true;
                ((Cell*) value)->a.dfs();
                ((Cell*) value)->b.dfs();
            }
            else
                return;
        }
    };

    struct Cell
    {

        bool marked: 1;
        Value a;
        Value b;
        
        Cell(): marked(false) {}
        Cell(signed long int _a, signed long int _b): a(_a), b(_b), marked(false) {}

    };


    char program_code[TORUS_X_SIZE][TORUS_Y_SIZE]; // Program code modelled as a 25x80 torus
    short int pcx, pcy; // Program counters for the x and y dimension of the program_code respectively
    std::vector<Interpreter::Value> program_stack;
    Cell *program_heap;
    Cell *freeList;
    

    enum Direction {
        Up,
        Down,
        Left,
        Right
    } pc_dir;


    // Program Counter alteration functions
    void inc_counter();

    // Stack pop function
    Value pop();

    // Garbage Collector functions
    void mark();
    void sweep();

    // Misc Helper Functions
    void print_torus();

public:
    Interpreter();
    ~Interpreter();

    bool load(std::string program_path);
    int execute();
};