#include "parallel_header.hpp"

template<typename BidirectionalIterator, class Compare>
class ParallelQuickSortSolver {
private:
    const BidirectionalIterator first, last;
    const Compare comp;
    std::mt19937 mt;
    static const unsigned long THRESHOLD = 15000;
    void parallel_partial_sort(const BidirectionalIterator _first, const BidirectionalIterator _last);

public:
    ParallelQuickSortSolver(const BidirectionalIterator _first, const BidirectionalIterator _last, const Compare _comp)
        : first(_first), last(_last), comp(_comp), mt(std::random_device{}()){}
    void operator()()
    {
        parallel_partial_sort(first, last);
    }
};


template<typename BidirectionalIterator, class Compare>
void ParallelQuickSortSolver<BidirectionalIterator, Compare>::parallel_partial_sort
    (const BidirectionalIterator _first, const BidirectionalIterator _last)
{
    const unsigned long length = std::distance(_first, _last);
    if(length <= THRESHOLD)
    {
        if(length >= 2)
            std::sort(_first, _last);
        return;
    }
    std::uniform_int_distribution<> uid(0, length);
    const auto pivot_itr = std::next(_first, uid(mt));
    const auto pivot = *pivot_itr;
    std::iter_swap(_first, pivot_itr);
    const BidirectionalIterator middle = std::partition(_first + 1, _last, std::bind(comp, std::placeholders::_1, std::cref(pivot)));
    std::iter_swap(_first, middle - 1);
    auto fut = std::async(std::launch::async, &ParallelQuickSortSolver<BidirectionalIterator, Compare>::parallel_partial_sort,
                            this, _first, middle - 1);
    parallel_partial_sort(middle, _last);
    fut.wait();
}

template<typename BidirectionalIterator, class Compare>
void parallel_quick_sort(const BidirectionalIterator first, const BidirectionalIterator last, const Compare comp)
{
    ParallelQuickSortSolver<BidirectionalIterator, Compare> solver(first, last, comp);
    solver();
}

template<typename BidirectionalIterator>
void parallel_quick_sort(const BidirectionalIterator first, const BidirectionalIterator last)
{
    parallel_quick_sort(first, last, std::less<typename BidirectionalIterator::value_type>());
}
