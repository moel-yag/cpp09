#ifndef BITCOIN_EXCHANGE_CPP
# define BITCOIN_EXCHANGE_CPP

# include <iostream>
# include <fstream>
# include <map>
# include <ctime>
#include <sstream>

class BitcoinExchange
{
    private:
        // std::map<std::string, double> input;
        std::map<std::string, double> db;
        std::string filename;
        static std::string db_filename;

    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange &other);
        BitcoinExchange &operator =(const BitcoinExchange &other);
        ~BitcoinExchange();

        std::string read_db();
        void read_input();
        std::string handle_db_line(std::string &line);
        void handle_input_line(std::string &line);
        void setInputname(std::string filename);
};


#endif
