#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(): db(), filename()
{

}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
    : db(other.db), filename(other.filename)
{}

BitcoinExchange &BitcoinExchange::operator =(const BitcoinExchange &other)
{
    if (this != &other)
    {
        this->db = other.db;
        this->filename = other.filename;
    }
    return (*this);
}

BitcoinExchange::~BitcoinExchange()
{
}

void BitcoinExchange::setInputname(std::string filename)
{
    this->filename = filename;
}

std::string  BitcoinExchange::db_filename = "data.csv";


static std::string trim(const std::string& s)
{
    std::size_t a = s.find_first_not_of(" \t");
    if (a == std::string::npos)
        return "";
    std::size_t b = s.find_last_not_of(" \t");
    return s.substr(a, b - a + 1);
}

static bool isLeap(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

bool parseDate(const std::string& str, std::tm& date)
{
    std::istringstream iss(str);
    char dash1, dash2;
    int y, m, d;

    iss >> y >> dash1 >> m >> dash2 >> d;
    // format exact "Y-M-D" et chaine entierement consommee
    if (iss.fail() || !iss.eof() || dash1 != '-' || dash2 != '-')
        return false;
    // plages valides
    if (y < 1 || m < 1 || m > 12 || d < 1 || d > 31)
        return false;
    static const int mdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxd = mdays[m - 1];
    if (m == 2 && isLeap(y))
        maxd = 29;
    if (d > maxd)
        return false;

    date.tm_year = y - 1900; // tm_year = années depuis 1900
    date.tm_mon = m - 1;     // tm_mon = 0..11
    date.tm_mday = d;
    return true;
}

std::string BitcoinExchange::handle_db_line(std::string &line)
{
    std::size_t pos = line.find(",");
    if (pos == std::string::npos)
        return "Error";
    std::string date_part = trim(line.substr(0, pos));
    std::tm date;

    // check if is date
    if (!parseDate(date_part, date))
        return "Date Invalide";

    //get the rate
    std::string rate_part = line.substr(pos + 1);
    std::istringstream iss(rate_part);
    double rate;
    if (!(iss >> rate) || !iss.eof())
        return "Nombre invalide";
    this->db.insert(std::make_pair(date_part, rate));
    
    return "";
}

void print_line_rate(std::map<std::string, double>::iterator &db_it, std::string &sdate, double rate)
{
    std::cout << (sdate) << " => " << rate << " = " << rate * (db_it->second) << std::endl;
}

void BitcoinExchange::handle_input_line(std::string &line)
{
    std::size_t pos = line.find("|");
    if (pos == std::string::npos)
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return ;
    }

    std::string sdate  = trim(line.substr(0, pos));
    std::string svalue = trim(line.substr(pos + 1));
    std::tm date;

    // check if is date
    if (!parseDate(sdate, date))
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return ;
    }

    std::istringstream issv(svalue);
    double rate;
    if (!(issv >> rate) || !issv.eof())
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return ;
    }
    if (rate < 0)
    {
        std::cerr << "Error: not a positive number." << std::endl;
        return ;
    }
    if (rate > 1000)
    {
        std::cerr << "Error: too large a number." << std::endl;
        return ;
    }

    std::map<std::string, double>::iterator it = this->db.lower_bound(sdate);
    if (it == this->db.end() || it->first != sdate)
    {
        if (it == this->db.begin())
        {
            std::cerr << "Error: no data before => " << sdate << std::endl;
            return ;
        }
        --it;
    }
    print_line_rate(it, sdate, rate);
}

std::string BitcoinExchange::read_db()
{
    std::ifstream db_file(BitcoinExchange::db_filename.c_str());

    if (!db_file.is_open())
        return "Error: could not open database file.";
    std::string line;
    std::getline(db_file, line);
    if (line != "date,exchange_rate")
    {
        db_file.close();
        return "Error: invalid head of database";
    }
    while (std::getline(db_file, line))
    {
        if (this->handle_db_line(line) != "")
        {
            db_file.close();
            std::cerr << "Error :" << line << std::endl;
            return "Database Error";
        }
    }
    db_file.close();
    return "";
}

void BitcoinExchange::read_input()
{

    std::ifstream input_file(this->filename.c_str());

    if (!input_file.is_open())
    {
        std::cerr << "Error: could not open input file." << std::endl;
        return ;
    }
    std::string line;
    std::getline(input_file, line);
    if (line != "date | value")
    {
        input_file.close();
        std::cerr << "Error: invalid head of input file" << std::endl;
        return ;
    }
    while (std::getline(input_file, line))
    {
        this->handle_input_line(line);
    }
}
