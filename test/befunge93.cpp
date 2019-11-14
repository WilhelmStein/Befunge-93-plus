#include "interpreter.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cout<<"Error: Wrong number of arguments\nUsage:befunge_93_exe_name program_path"<<std::endl;
        return -1;
    }

    Interpreter befunge_93;
    // befunge93.load(std::string(argv[1]));
    return befunge_93.execute();
}