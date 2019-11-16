#include "interpreter.hpp"
#include "opcodes.hpp"
#include <stack>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

using std::string;
using std::cout;
using std::cin;
using std::endl;

#define NEXT_INSTRUCTION(labels, code, pcx, pcy) \
    goto *(void *)(labels[code[pcx][pcy]])

// Program Counter alteration function
void Interpreter::inc_counter()
{
    switch(pc_dir)
    {
        case Direction::Up:     ( (pcx - 1) < 0 ) ? (pcx = TORUS_X_SIZE - 1) : ( pcx-- );    break;
        case Direction::Down:   ( (pcx + 1) >= TORUS_X_SIZE ) ? (pcx = 0) : ( pcx++ );    break;
        case Direction::Left:   ( (pcy - 1) < 0 ) ? (pcy = TORUS_Y_SIZE - 1) : ( pcy-- );     break;
        case Direction::Right:  ( (pcy + 1) >= TORUS_Y_SIZE ) ? (pcy = 0) : ( pcy++ );    break;
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

bool Interpreter::load(string program_path) // Remember to add some boundary checks and make the loading more efficient
{
    std::ifstream input_file;
    input_file.open(program_path);

    if(!input_file.is_open())
    {
        cout<<"Error: Could not open file with path '" << program_path << "'"<<endl;
        return false;
    }

    std::stringstream str_stream;
    str_stream << input_file.rdbuf();
    string code = str_stream.str();

    
    for(size_t i = 0, j = 0, k = 0; k < code.length(); k++)
    {
        if(i > TORUS_X_SIZE || j > TORUS_Y_SIZE)
        {
            cout<<"Error: Program file too big."<<endl;
            return false;
        }

        if(code[k] == '\n')
        {
            i++;
            j = 0;
            continue;
        }

        program_code[i][j++] = code[k];
    }

    //print_torus();
    return true;
}

int Interpreter::execute()
{
    static void* labels[] = {
        &&add_label,
        &&multiply_label,
        &&subtract_label,
        &&divide_label,
        &&modulo_label,
        &&duplicate_label,
        &&swap_label,
        &&pop_label,
        &&negation_label,
        &&greater_label,
        &&right_label,
        &&left_label,
        &&up_label,
        &&down_label,
        &&random_label,
        &&bridge_label,
        &&h_if_label,
        &&v_if_label,
        &&in_d_label,
        &&in_c_label,
        &&out_d_label,
        &&out_c_label,
        &&string_mode_label_start,
        &&string_mode_label_push,
        &&string_mode_label_end,
        &&get_label,
        &&put_label,
        &&exit_label,
        &&empty_label
    };

    bool string_mode = false;

    std::cout<<"Befunge-93 Interpreter/Debugger v2.25"<<std::endl;

    while (true)
    {
        char opcode = program_code[pcx][pcy];

        if(string_mode)
        {
            switch (opcode)
            {
                case STRING_MODE: string_mode_label_end:    string_mode = false; break;
                default:          string_mode_label_push:   program_stack.push(opcode); break;
            }
        }
        else
        {
            switch(opcode)
            {
                case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '0': program_stack.push(opcode - '0'); break;


                // Stack Arithmetic
                case ADD:       add_label:      program_stack.push(pop() + pop()); break;
                case MULTIPLY:  multiply_label: program_stack.push(pop() * pop()); break;
                case SUBTRACT:  subtract_label:
                {
                    signed long int b = pop(), a = pop(); 
                    program_stack.push(a - b);
                    break;
                }
                case DIVIDE: divide_label:
                {
                    signed long int b = pop(), a = pop();
                    program_stack.push((b == 0) ? (a) : (a / b));
                    break;
                }
                case MODULO: modulo_label:
                {
                    signed long int b = pop(), a = pop();
                    program_stack.push((b == 0) ? (a) : (a % b));
                    break;
                }
                case DUPLICATE: duplicate_label:
                {
                    signed long int a = pop();
                    program_stack.push(a);
                    program_stack.push(a); 
                    break;
                }
                case SWAP: swap_label:
                {
                    signed long int b = pop(), a = pop();
                    program_stack.push(b);
                    program_stack.push(a);
                    break;
                }
                case POP:       pop_label:      pop(); break;
                case NEGATION:  negation_label: program_stack.push((pop() == 0) ? (1) : (0)); break;
                case GREATER:   greater_label:  program_stack.push((pop() < pop()) ? (1) : (0)); break;


                // Program Counter Movement 
                case RIGHT:     right_label:    pc_dir = Direction::Right; break;
                case LEFT:      left_label:     pc_dir = Direction::Left;  break; 
                case UP:        up_label:       pc_dir = Direction::Up;    break; 
                case DOWN:      down_label:     pc_dir = Direction::Down;  break;
                case RANDOM:    random_label:   pc_dir = Direction(std::rand() % 4); break;
                case BRIDGE:    bridge_label:   inc_counter(); break;


                // Program Control Flow
                case H_IF: h_if_label: (pop() == 0) ? (pc_dir = Direction::Right) : (pc_dir = Direction::Left); break;
                case V_IF: v_if_label: (pop() == 0) ? (pc_dir = Direction::Down)  : (pc_dir = Direction::Up); break;


                // I/O
                case IN_D: in_d_label:
                {
                    signed long int v;
                    cin>>v;
                    program_stack.push(v);
                    break;
                }
                case IN_C: in_c_label:
                {
                    program_stack.push(getchar());
                    break;
                }
                case OUT_D:         out_d_label:                std::cout<<pop()<<' '; break;
                case OUT_C:         out_c_label:                std::cout<<(char)pop(); break;
                case STRING_MODE:   string_mode_label_start:    string_mode = true; break;


                // Alter Program Memory
                case GET: get_label:
                {
                    signed long int x = pop(), y = pop();

                    if(y >= TORUS_Y_SIZE || y < 0 || x >= TORUS_X_SIZE || x < 0)
                    {
                        cout<<"Error: Get command out of bounds (" << x << ", " << y << ")"<<endl;
                        break;
                    }
                    
                    program_stack.push(program_code[x][y]);
                    break;
                }
                case PUT: put_label:
                {  
                    signed long int x = pop(), y = pop(), v = pop();

                    if(y >= TORUS_Y_SIZE || y < 0 || x >= TORUS_X_SIZE || x < 0)
                    {
                        cout<<"Error: Put command out of bounds (" << x << ", " << y << ")"<<endl;
                        break;
                    }

                    program_code[x][y] = v;
                    break;
                }
            

                // Misc
                case EXIT:  exit_label:  return 0;
                case EMPTY: empty_label: break;


                default:
                    //cout<<"Error: Unknown command '"<< opcode <<"' encountered."<<endl;
                    break;
            }   
        }
        inc_counter();
    }
}