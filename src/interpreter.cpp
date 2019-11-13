#include "interpreter.hpp"
#include "opcodes.hpp"
#include <stack>
#include <iostream>

using std::string;

// Program Counter alteration functions
void Interpreter::inc_xcounter() { ( (pcx + 1) > TORUS_X_SIZE ) ? (pcx = 0) : ( pcx++ ); }
void Interpreter::red_xcounter() { ( (pcx - 1) < 0 ) ? (pcx = TORUS_X_SIZE) : ( pcx-- ); }
void Interpreter::inc_ycounter() { ( (pcx + 1) > TORUS_Y_SIZE ) ? (pcy = 0) : ( pcy++ ); }
void Interpreter::red_ycounter() { ( (pcx - 1) < 0 ) ? (pcy = TORUS_Y_SIZE) : ( pcy-- ); }

// Stack Pop function
signed long int Interpreter::pop()
{
    signed long int item = program_stack.top();
    program_stack.pop();
    return item;
}

Interpreter::Interpreter()
{
    pcx = pcy = 0;

    for (unsigned i = 0; i < 80; i++)
        for (unsigned j = 0; j < 25; j++)
            program_code[i][j] = ' ';
}

Interpreter::~Interpreter() {}


int Interpreter::execute(string program_name)
{
    
    // PROGRAM LOADER GOES HERE

    while (true)
    {
        char opcode = program_code[pcx][pcy];

        switch(opcode)
        {
            case ADD: case SUBTRACT: case MULTIPLY: case DIVIDE: case MODULO:
                signed long int a,b;

                a = program_stack.top();
                program_stack.pop();

                b = program_stack.top();
                program_stack.pop();

                switch(opcode)
                {
                    case ADD: program_stack.push(a + b); break;
                    case SUBTRACT: program_stack.push(a - b); break;
                    case MULTIPLY: program_stack.push(a * b); break;
                    case DIVIDE: program_stack.push(a / b); break;
                    case MODULO: program_stack.push(a % b); break;
                }
                inc_ycounter();
                break;

            case RIGHT:  inc_ycounter(); break;
            case LEFT:   red_ycounter(); break; 
            case UP:     inc_xcounter(); break; 
            case DOWN:   red_xcounter(); break;
            
            case IN_D:  break;
            case IN_C:  break;
            case OUT_D: std::cout<<pop(); inc_ycounter(); break;
            case OUT_C: std::cout<<(char)pop(); inc_ycounter(); break;

            case EXIT:
                return 0;
            
            case NULL: inc_ycounter(); break;

            default:
                std::cout<<"Error: Unknown command '"<< opcode <<"' encountered."<<std::endl;
                return -1;
        }
    }
}