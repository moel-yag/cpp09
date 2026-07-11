#include "PmergeMe.hpp"

int main(int argc, char** argv)
{
    try
    {
        PmergeMe p;
        p.parse(argc, argv);
        p.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
