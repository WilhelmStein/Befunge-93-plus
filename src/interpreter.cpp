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
    goto *(void *)((labels)[(size_t)(code)[(pcx)][(pcy)]])

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
inline Interpreter::Value Interpreter::pop()
{
    Interpreter::Value item;

    if(!program_stack.empty())
    {
        item = program_stack.back();
        program_stack.pop_back();
    }

    return item;
}

// Garbace Collector functions

inline void Interpreter::mark()
{
    for (std::vector<Value>::iterator it = program_stack.begin(); it != program_stack.end(); it++)
        it->dfs();
}


inline void Interpreter::sweep()
{
    
    for(size_t i = 0; i < HEAP_SIZE; i++)
    {
        if(program_heap[i].marked)
            program_heap[i].marked = false;
        else
        {
            program_heap[i].a.value = (signed long int)freeList;
            freeList = &(program_heap[i]);
        }
    }
    
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
    program_heap = new Cell[HEAP_SIZE];

    freeList = program_heap;
    for(size_t i = 0; i < HEAP_SIZE - 1; i++)
        program_heap[i].a.value = (signed long int)&program_heap[i + 1];
    
    program_heap[HEAP_SIZE - 1].a.value = 0;
    

    pcx = pcy = 0;
    pc_dir = Direction::Right;

    for (unsigned i = 0; i < TORUS_X_SIZE; i++)
        for (unsigned j = 0; j < TORUS_Y_SIZE; j++)
            program_code[i][j] = ' ';
}

Interpreter::~Interpreter() { delete[] program_heap;}

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
        if(code[k] == '\n')
        {
            i++;
            j = 0;
            continue;
        }

        if(i >= TORUS_X_SIZE || j >= TORUS_Y_SIZE)
        {
            cout<<"Error: Program file too big."<<endl;
            return false;
        }

        program_code[i][j++] = code[k];
    }

    //print_torus();
    return true;
}

int Interpreter::execute()
{
    static void *labels[128], *str_mode_labels[128];
    
    for(size_t i = 0; i < 128; i++)
    {
        labels[i] = &&undefined_label;
        str_mode_labels[i] = &&string_mode_push_label;
    }

    labels[ADD] = &&add_label;
    labels[MULTIPLY] = &&multiply_label;
    labels[SUBTRACT] = &&subtract_label;
    labels[DIVIDE] = &&divide_label;
    labels[MODULO] = &&modulo_label;
    labels[DUPLICATE] = &&duplicate_label;
    labels[SWAP] = &&swap_label;
    labels[POP] = &&pop_label;
    labels[NEGATION] = &&negation_label;
    labels[GREATER] = &&greater_label;
    labels[RIGHT] = &&right_label;
    labels[LEFT] = &&left_label;
    labels[UP] = &&up_label;
    labels[DOWN] = &&down_label;
    labels[RANDOM] = &&random_label;
    labels[BRIDGE] = &&bridge_label;
    labels[H_IF] = &&h_if_label;
    labels[V_IF] = &&v_if_label;
    labels[IN_D] = &&in_d_label;
    labels[IN_C] = &&in_c_label;
    labels[OUT_D] = &&out_d_label;
    labels[OUT_C] = &&out_c_label;
    labels[STRING_MODE] = &&string_mode_start_label;
    labels[GET] = &&get_label;
    labels[PUT] = &&put_label;
    labels[CONS] = &&cons_label;
    labels[HEAD] = &&head_label;
    labels[TAIL] = &&tail_label;
    labels[EXIT] = &&exit_label;
    labels[EMPTY] = &&empty_label;
    
    for(size_t i = '0'; i <= '9'; i++)
        labels[i] = &&push_number_label;

    str_mode_labels[STRING_MODE] = &&string_mode_end_label;

    bool string_mode = false;

    std::cout<<"Befunge-93 Interpreter/Debugger v2.25"<<std::endl;

    while (true)
    {
        char opcode = program_code[pcx][pcy];

        if(string_mode)
        {
            switch (opcode)
            {
                case STRING_MODE: string_mode_end_label:    string_mode = false; break;
                default:          string_mode_push_label:   program_stack.push_back(opcode); break;
            }
        }
        else
        {
            switch(opcode)
            {
                case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '0': push_number_label: program_stack.push_back(opcode - '0'); break;


                // Stack Arithmetic
                case ADD:       add_label:      program_stack.push_back(pop() + pop()); break;
                case MULTIPLY:  multiply_label: program_stack.push_back(pop() * pop()); break;
                case SUBTRACT:  subtract_label:
                {
                    signed long int b = pop(), a = pop(); 
                    program_stack.push_back(a - b);
                    break;
                }
                case DIVIDE: divide_label:
                {
                    signed long int b = pop(), a = pop();
                    program_stack.push_back((b == 0) ? (a) : (a / b));
                    break;
                }
                case MODULO: modulo_label:
                {
                    signed long int b = pop(), a = pop();
                    program_stack.push_back((b == 0) ? (a) : (a % b));
                    break;
                }
                case DUPLICATE: duplicate_label:
                {
                    auto a = pop();
                    program_stack.push_back(a);
                    program_stack.push_back(a); 
                    break;
                }
                case SWAP: swap_label:
                {
                    auto b = pop(), a = pop();
                    program_stack.push_back(b);
                    program_stack.push_back(a);
                    break;
                }
                case POP:       pop_label:      pop(); break;
                case NEGATION:  negation_label: program_stack.push_back((pop() == 0) ? (1) : (0)); break;
                case GREATER:   greater_label:  program_stack.push_back((pop() < pop()) ? (1) : (0)); break;


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
                    program_stack.push_back(v);
                    break;
                }
                case IN_C: in_c_label:
                {
                    program_stack.push_back(getchar());
                    break;
                }
                case OUT_D:         out_d_label:                std::cout<<pop()<<' '<<std::flush; break;
                case OUT_C:         out_c_label:                std::cout<<(char)pop()<<std::flush; break;
                case STRING_MODE:   string_mode_start_label:    string_mode = true; break;


                // Alter Program Memory
                case GET: get_label:
                {
                    signed long int x = pop(), y = pop();

                    if(y >= TORUS_Y_SIZE || y < 0 || x >= TORUS_X_SIZE || x < 0)
                    {
                        cout<<"Error: Get command out of bounds (" << x << ", " << y << ")"<<endl;
                        break;
                    }
                    
                    program_stack.push_back(program_code[x][y]);
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


                // Heap Commands
                case CONS: cons_label:
                {
                    if(freeList == 0)
                    {
                        mark();
                        sweep();

                        if(freeList == 0)
                        {
                            cout<<"Error:Heap Overflow"<<endl;
                            return -1;
                        }
                    }

                    Value b = pop(), a = pop();
                    Value s(a, b, freeList);
                    program_stack.push_back(s);
                    
                    break;
                }
                case HEAD: head_label: program_stack.push_back(((Cell*)(pop().value))->a); break;
                case TAIL: tail_label: program_stack.push_back(((Cell*)(pop().value))->b); break;
            

                // Misc
                case EXIT:  exit_label:  return 0;
                case EMPTY: empty_label: break;


                default: undefined_label:
                    //cout<<"Error: Unknown command '"<< opcode <<"' encountered."<<endl;
                    break;
            }   
        }
        inc_counter();
        opcode = program_code[pcx][pcy];
        NEXT_INSTRUCTION(((string_mode) ? (str_mode_labels) : (labels)), program_code, pcx, pcy);
    }
}