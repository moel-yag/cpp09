#ifndef P_MERGE_ME_HPP
# define P_MERGE_ME_HPP

# include <vector>
# include <deque>
# include <string>
# include <iostream>
# include <stdexcept>
# include <cstddef>

class PmergeMe
{
    private:
        std::vector<unsigned int> _input;

    public:
        PmergeMe();
        PmergeMe(const PmergeMe& other);
        PmergeMe& operator=(const PmergeMe& other);
        ~PmergeMe();

        void parse(int argc, char** argv);
        void run() const;
};

template <typename Cont>
void printSequence(const std::string& label, const Cont& c)
{
    std::cout << label;
    for (typename Cont::const_iterator it = c.begin(); it != c.end(); ++it)
        std::cout << " " << *it;
    std::cout << std::endl;
}

template <typename Cont>
struct Group
{
    Cont values;

    unsigned int max() const { return values.back(); }
};

template <typename Cont>
static std::size_t lowerBoundByMax(const std::vector<Group<Cont> >& chain,
                                   std::size_t lo, std::size_t hi,
                                   unsigned int key)
{
    while (lo < hi)
    {
        std::size_t mid = lo + (hi - lo) / 2;
        if (chain[mid].max() < key)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

inline std::vector<std::size_t> jacobsthalInsertionOrder(std::size_t count)
{
    std::vector<std::size_t> order;
    if (count < 2)
        return order;

    std::size_t prevJ = 1; // J(2)
    std::size_t curJ = 3;  // J(3)
    std::size_t last = count;

    while (prevJ + 1 <= last)
    {
        std::size_t upper = (curJ < last) ? curJ : last;
        std::size_t lower = prevJ + 1;
        for (std::size_t idx = upper; ; --idx)
        {
            order.push_back(idx - 1);
            if (idx == lower)
                break;
        }
        std::size_t nextJ = curJ + 2 * prevJ;
        prevJ = curJ;
        curJ = nextJ;
    }
    return order;
}

template <typename Cont>
static std::vector<Group<Cont> > fjSort(std::vector<Group<Cont> > level)
{
    if (level.size() < 2)
        return level;

    bool hasStray = (level.size() % 2 != 0);
    Group<Cont> stray;
    if (hasStray)
    {
        stray = level.back();
        level.pop_back();
    }

    std::vector<Group<Cont> > merged;
    merged.reserve(level.size() / 2);
    for (std::size_t i = 0; i + 1 < level.size(); i += 2)
    {
        Group<Cont> left = level[i];
        Group<Cont> right = level[i + 1];
        if (left.max() > right.max())
        {
            Group<Cont> tmp = left;
            left = right;
            right = tmp;
        }
        Group<Cont> block;
        block.values = left.values;
        for (typename Cont::const_iterator it = right.values.begin();
             it != right.values.end(); ++it)
            block.values.push_back(*it);
        merged.push_back(block);
    }

    std::vector<Group<Cont> > sorted = fjSort(merged);

    std::size_t pairs = sorted.size();
    std::size_t half = sorted[0].values.size() / 2;
    std::vector<Group<Cont> > aChain;
    std::vector<Group<Cont> > bPend;
    aChain.reserve(pairs);
    bPend.reserve(pairs);
    for (std::size_t i = 0; i < pairs; ++i)
    {
        Group<Cont> b;
        Group<Cont> a;
        typename Cont::const_iterator it = sorted[i].values.begin();
        for (std::size_t k = 0; k < half; ++k, ++it)
            b.values.push_back(*it);
        for (std::size_t k = 0; k < half; ++k, ++it)
            a.values.push_back(*it);
        bPend.push_back(b);
        aChain.push_back(a);
    }

    std::vector<Group<Cont> > chain;
    chain.reserve(pairs * 2 + 1);
    chain.push_back(bPend[0]);
    for (std::size_t i = 0; i < pairs; ++i)
        chain.push_back(aChain[i]);

    std::vector<std::size_t> aPos(pairs);
    for (std::size_t i = 0; i < pairs; ++i)
        aPos[i] = i + 1;

    std::size_t pendCount = pairs + (hasStray ? 1 : 0);
    std::vector<std::size_t> order = jacobsthalInsertionOrder(pendCount);
    for (std::size_t o = 0; o < order.size(); ++o)
    {
        std::size_t j = order[o];
        const Group<Cont>& elem = (j < pairs) ? bPend[j] : stray;
        std::size_t bound = (j < pairs) ? aPos[j] : chain.size();
        std::size_t pos = lowerBoundByMax(chain, 0, bound, elem.max());
        chain.insert(chain.begin() + pos, elem);
        for (std::size_t t = 0; t < pairs; ++t)
            if (aPos[t] >= pos)
                ++aPos[t];
    }

    return chain;
}

template <typename Cont>
void fordJohnsonSort(Cont& data)
{
    if (data.size() < 2)
        return;

    std::vector<Group<Cont> > level;
    level.reserve(data.size());
    for (typename Cont::const_iterator it = data.begin(); it != data.end(); ++it)
    {
        Group<Cont> g;
        g.values.push_back(*it);
        level.push_back(g);
    }

    std::vector<Group<Cont> > sorted = fjSort(level);

    data.clear();
    for (std::size_t i = 0; i < sorted.size(); ++i)
        for (typename Cont::const_iterator it = sorted[i].values.begin();
             it != sorted[i].values.end(); ++it)
            data.push_back(*it);
}

#endif
