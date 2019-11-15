#include "interpreter.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        std::cout<<"Error: Wrong number of arguments\nUsage:befunge_93_exe_name program_path"<<std::endl;
        return -1;
    }

    std::cout<<"Loading File: "<< argv[1]<<std::endl;

    Interpreter befunge_93;
    return (befunge_93.load(argv[1])) ? (befunge_93.execute()) : (-1);
}