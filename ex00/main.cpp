#include "BitcoinExchange.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: ./btc [input_file]\n";
        return 1;
    }

    BitcoinExchange bc;
    bc.setInputname(argv[1]);
    std::string err = bc.read_db();
    if (!err.empty())
    {
        std::cerr << err << std::endl;
        return 1;
    }
    bc.read_input();
    return (0);
}