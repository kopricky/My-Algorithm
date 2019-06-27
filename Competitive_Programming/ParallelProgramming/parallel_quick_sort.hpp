#include "parallel_header.hpp"

const unsigned long THRESHOLD = 15000;

template<typename Iterator>
void parallel_partial_sort(const Iterator first, const Iterator last, std::mt19937& mt)
{
    const unsigned long length = std::distance(first, last);
    if(length <= THRESHOLD)
    {
        if(length >= 2)
            std::sort(first, last);
        return;
    }
    std::uniform_int_distribution<> uid(0, length);
    auto pivot = *std::next(first, uid(mt));
    Iterator middle1 = std::partition(first, last, [pivot](const auto& itr){ return itr < pivot; });
    Iterator middle2 = std::partition(first, last, [pivot](const auto& itr){ return !(pivot < itr); });
    auto fut = std::async(std::launch::async, parallel_partial_sort<Iterator>, first, middle1, std::ref(mt));
    parallel_partial_sort(middle2, last, mt);
    fut.wait();
}

template<typename Iterator>
void parallel_quick_sort(Iterator first, Iterator last)
{
    std::mt19937 mt(std::random_device{}());
    parallel_partial_sort(first, last, mt);
}
