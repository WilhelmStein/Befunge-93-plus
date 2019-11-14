#include "interpreter.hpp"
#include "opcodes.hpp"
#include <stack>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

using std::string;
using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;

// Program Counter alteration functions
void Interpreter::inc_xcounter() { ( (pcx + 1) > TORUS_X_SIZE ) ? (pcx = 0) : ( pcx++ ); }
void Interpreter::red_xcounter() { ( (pcx - 1) < 0 ) ? (pcx = TORUS_X_SIZE) : ( pcx-- ); }
void Interpreter::inc_ycounter() { ( (pcx + 1) > TORUS_Y_SIZE ) ? (pcy = 0) : ( pcy++ ); }
void Interpreter::red_ycounter() { ( (pcx - 1) < 0 ) ? (pcy = TORUS_Y_SIZE) : ( pcy-- ); }

void Interpreter::inc_counter()
{
    switch(pc_dir)
    {
        case Direction::Up:     ( (pcx - 1) < 0 ) ? (pcx = TORUS_X_SIZE) : ( pcx-- );    break;
        case Direction::Down:   ( (pcx + 1) > TORUS_X_SIZE ) ? (pcx = 0) : ( pcx++ );    break;
        case Direction::Left:   ( (pcx - 1) < 0 ) ? (pcy = TORUS_Y_SIZE) : ( pcy-- );     break;
        case Direction::Right:  ( (pcx + 1) > TORUS_Y_SIZE ) ? (pcy = 0) : ( pcy++ );    break;
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

// Misc Helper Functions
void Interpreter::print_torus()
{
    for (unsigned i = 0; i < TORUS_X_SIZE; i++)
    {
        for (unsigned j = 0; j < TORUS_Y_SIZE; j++)
            cout<<program_code[i][j];
        cout<<std::endl;
    }
}

Interpreter::Interpreter()
{
    std::srand(std::time(nullptr));

    pcx = pcy = 0;
    pc_dir = Direction::Right;

    for (unsigned i = 0; i < TORUS_X_SIZE; i++)
        for (unsigned j = 0; j < TORUS_Y_SIZE; j++)
            program_code[i][j] = ' ';
}

Interpreter::~Interpreter() {}

void Interpreter::load(string program_path) // Remember to add some boundary checks and make the loading more efficient
{
    ifstream input_file;
    input_file.open(program_path);

    stringstream str_stream;
    str_stream << input_file.rdbuf();
    string code = str_stream.str();

    
    for(size_t i = 0, j = 0, k = 0; k < code.length(); k++, j++)
    {
        if(code[k] == '\n')
        {
            i++;
            j = 0;
            k++;
        }

        program_code[i][j] = code[k];
    }

    //print_torus();
}

int Interpreter::execute()
{
    bool string_mode = false;

    while (true)
    {
        char opcode = program_code[pcx][pcy];

        if(string_mode)
        {
            switch (opcode)
            {
                case STRING_MODE: string_mode = false;  break;
                default: program_stack.push(opcode);    break;
            }
        }
        else
        {
            switch(opcode)
            {
                case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '0': program_stack.push(atoi(&opcode)); break;


                // Stack Arithmetic
                case ADD:      program_stack.push(pop() + pop()); break;
                case MULTIPLY: program_stack.push(pop() * pop()); break;

                case SUBTRACT:
                {
                    signed long int b = pop(), a = pop(); 
                    program_stack.push(a - b);
                    break;
                }
                case DIVIDE:
                {
                    signed long int b = pop(), a = pop();
                    program_stack.push((b == 0) ? (a) : (a / b));
                    break;
                }
                case MODULO:
                {
                    signed long int b = pop(), a = pop();
                    program_stack.push((b == 0) ? (a) : (a % b));
                    break;
                }
                case DUPLICATE:
                {
                    signed long int a = pop();
                    program_stack.push(a);
                    program_stack.push(a); 
                    break;
                }
                case SWAP:
                {
                    signed long int b = pop(), a = pop();
                    program_stack.push(b);
                    program_stack.push(a);
                    break;
                }
                case POP: pop(); break;
                case NEGATION: program_stack.push((pop() == 0) ? (1) : (0)); break;
                case GREATER:  program_stack.push((pop() > pop()) ? (1) : (0)); break;


                // Program Counter Movement 
                case RIGHT:  pc_dir = Direction::Right; break;
                case LEFT:   pc_dir = Direction::Left;  break; 
                case UP:     pc_dir = Direction::Up;    break; 
                case DOWN:   pc_dir = Direction::Down;  break;
                case RANDOM: pc_dir = Direction(std::rand() % 4); break;
                case BRIDGE: inc_counter(); break;


                // Program Control Flow
                case H_IF: (pop() == 0) ? (pc_dir = Direction::Right) : (pc_dir = Direction::Left); break;
                case V_IF: (pop() == 0) ? (pc_dir = Direction::Down)  : (pc_dir = Direction::Up); break;


                // I/O
                case IN_D:  break;
                case IN_C:  break;
                case OUT_D: std::cout<<pop(); break;
                case OUT_C: std::cout<<(char)pop(); break;
                case STRING_MODE: string_mode = true; break;


                case EXIT: return 0;
                case EMPTY: break;


                default:
                    cout<<"Error: Unknown command '"<< opcode <<"' encountered."<<endl;
                    return -1;
            }   
        }
        inc_counter();
    }
}