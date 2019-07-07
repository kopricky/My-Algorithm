#include "parallel_header.hpp"

template<typename ForwardIterator, typename T, class AssociativeBinaryOperation=std::plus<T> >
T parallel_accumulate(const ForwardIterator first, const ForwardIterator last, const T init, const AssociativeBinaryOperation op=AssociativeBinaryOperation())
{
    const unsigned long length = std::distance(first, last);
    if(!length)
        return init;
    const unsigned long min_per_thread = 100;
    const unsigned long max_threads = (length + min_per_thread - 1) / min_per_thread;
    const unsigned long hardware_threads = std::thread::hardware_concurrency();
    const unsigned long num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    const unsigned long block_size = length / num_threads;
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);
    ForwardIterator block_start = first;
    auto accumulate_block = [op](ForwardIterator first, ForwardIterator last, T& result)
    {
        result = std::accumulate(first, last, result, op);
    };
    for(unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        ForwardIterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate_block, block_start, block_end, std::ref(results[i]));
        block_start = block_end;
    }
    accumulate_block(block_start, last, results[num_threads - 1]);
    for(auto& entry : threads)
        entry.join();
    return std::accumulate(results.begin(), results.end(), init, op);
};
