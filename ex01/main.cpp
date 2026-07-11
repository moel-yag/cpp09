#include "RPN.hpp"
#include <iostream>
#include <exception>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    try
    {
        RPN rpn;
        std::cout << rpn.evaluate(argv[1]) << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    return 0;
}
