#include "PmergeMe.hpp"
#include <climits>
#include <cstdlib>
#include <cctype>
#include <sys/time.h>

PmergeMe::PmergeMe() : _input() {}

PmergeMe::PmergeMe(const PmergeMe& other) : _input(other._input) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
    if (this != &other)
        _input = other._input;
    return *this;
}

PmergeMe::~PmergeMe() {}

void PmergeMe::parse(int argc, char** argv)
{
    if (argc < 2)
        throw std::runtime_error("Error");

    for (int i = 1; i < argc; ++i)
    {
        std::string s(argv[i]);
        if (s.empty())
            throw std::runtime_error("Error");
        for (std::size_t k = 0; k < s.size(); ++k)
            if (!std::isdigit(static_cast<unsigned char>(s[k])))
                throw std::runtime_error("Error");

        char* end = 0;
        long v = std::strtol(s.c_str(), &end, 10);
        if (*end != '\0' || v < 0 || v > INT_MAX)
            throw std::runtime_error("Error");
        _input.push_back(static_cast<unsigned int>(v));
    }
}

static double elapsedUs(const timeval& a, const timeval& b)
{
    return (b.tv_sec - a.tv_sec) * 1000000.0 + (b.tv_usec - a.tv_usec);
}

void PmergeMe::run() const
{
    std::vector<unsigned int> vec(_input.begin(), _input.end());
    std::deque<unsigned int>  deq(_input.begin(), _input.end());

    printSequence("Before:", _input);

    timeval t0, t1;

    gettimeofday(&t0, NULL);
    fordJohnsonSort(vec);
    gettimeofday(&t1, NULL);
    double timeVector = elapsedUs(t0, t1);

    gettimeofday(&t0, NULL);
    fordJohnsonSort(deq);
    gettimeofday(&t1, NULL);
    double timeDeque = elapsedUs(t0, t1);

    printSequence("After: ", vec);

    std::cout << "Time to process a range of " << _input.size()
              << " elements with std::vector : " << timeVector << " us" << std::endl;
    std::cout << "Time to process a range of " << _input.size()
              << " elements with std::deque : " << timeDeque << " us" << std::endl;
}
