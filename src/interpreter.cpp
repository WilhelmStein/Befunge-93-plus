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

void Interpreter::inc_counter()
{
    switch(pc_dir)
    {
        case Direction::Up:     ( (pcx + 1) > TORUS_X_SIZE ) ? (pcx = 0) : ( pcx++ );    break;
        case Direction::Down:   ( (pcx - 1) < 0 ) ? (pcx = TORUS_X_SIZE) : ( pcx-- );    break;
        case Direction::Left:   ( (pcx + 1) > TORUS_Y_SIZE ) ? (pcy = 0) : ( pcy++ );    break;
        case Direction::Right:  ( (pcx - 1) < 0 ) ? (pcy = TORUS_Y_SIZE) : ( pcy-- );    break;
    }
}

// Stack Pop function
signed long int Interpreter::pop()
{
    signed long int item;

    if(program_stack.empty())
    {
        item = 0;
    }
    else
    {
        item = program_stack.top();
        program_stack.pop();
    }

    return item;
}

Interpreter::Interpreter()
{
    pcx = pcy = 0;
    pc_dir = Direction::Right;

    for (unsigned i = 0; i < TORUS_X_SIZE; i++)
        for (unsigned j = 0; j < TORUS_Y_SIZE; j++)
            program_code[i][j] = ' ';
}

Interpreter::~Interpreter() {}

void Interpreter::load(std::string program_path)
{
    
}

int Interpreter::execute()
{

    while (true)
    {
        char opcode = program_code[pcx][pcy];

        switch(opcode)
        {
            case ADD: case SUBTRACT: case MULTIPLY: case DIVIDE: case MODULO:
                signed long int a,b;

                a = pop();
                b = pop();

                switch(opcode)
                {
                    case ADD:       program_stack.push(a + b); break;
                    case SUBTRACT:  program_stack.push(a - b); break;
                    case MULTIPLY:  program_stack.push(a * b); break;
                    case DIVIDE:    program_stack.push(a / b); break;
                    case MODULO:    program_stack.push(a % b); break;
                }
                break;

            case RIGHT:  pc_dir = Direction::Right; break;
            case LEFT:   pc_dir = Direction::Left;  break; 
            case UP:     pc_dir = Direction::Up;    break; 
            case DOWN:   pc_dir = Direction::Down;  break;
            
            case IN_D:  break;
            case IN_C:  break;
            case OUT_D: std::cout<<pop(); break;
            case OUT_C: std::cout<<(char)pop(); break;

            case EXIT: return 0;
            case EMPTY: break;

            default:
                std::cout<<"Error: Unknown command '"<< opcode <<"' encountered."<<std::endl;
                return -1;
        }
        inc_counter();
    }
}