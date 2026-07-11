#include "RPN.hpp"
#include <sstream>
#include <stdexcept>
#include <cctype>

RPN::RPN() : _stack() {}

RPN::RPN(const RPN& other) : _stack(other._stack) {}

RPN& RPN::operator=(const RPN& other)
{
    if (this != &other)
        _stack = other._stack;
    return *this;
}

RPN::~RPN() {}

bool RPN::isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int RPN::applyOp(int a, int b, char op) const
{
    switch (op)
    {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0)
                throw std::runtime_error("Error"); // division par zero
            return a / b;
    }
    throw std::runtime_error("Error");
}

int RPN::evaluate(const std::string& expr)
{
    std::istringstream iss(expr);
    std::string token;

    while (iss >> token)
    {
        if (token.size() == 1 && std::isdigit(static_cast<unsigned char>(token[0])))
        {
            _stack.push(token[0] - '0');
        }
        else if (token.size() == 1 && isOperator(token[0]))
        {
            if (_stack.size() < 2)
                throw std::runtime_error("Error"); // pas assez d'operandes
            int b = _stack.top(); _stack.pop();
            int a = _stack.top(); _stack.pop();
            _stack.push(applyOp(a, b, token[0]));
        }
        else
        {
            throw std::runtime_error("Error");
        }
    }

    if (_stack.size() != 1)
        throw std::runtime_error("Error");
    return _stack.top();
}
