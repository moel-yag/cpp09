#ifndef RPN_HPP
# define RPN_HPP

# include <stack>
# include <string>

class RPN
{
    private:
        std::stack<int> _stack;

        static bool isOperator(char c);
        int applyOp(int a, int b, char op) const;

    public:
        RPN();
        RPN(const RPN& other);
        RPN& operator=(const RPN& other);
        ~RPN();

        int evaluate(const std::string& expr);
};

#endif
