#include "parallel_header.hpp"

template<class Function, typename Iterator>
void parallel_call(const Function&& f, const Iterator first, const Iterator last)
{
    const unsigned long length = std::distance(first, last);
    if(!length)
        return;
    const unsigned long min_per_thread = 25;
    const unsigned long max_threads = (length + min_per_thread - 1) / min_per_thread;
    const unsigned long hardware_threads = std::thread::hardware_concurrency();
    const unsigned long num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    const unsigned long block_size = length / num_threads;
    std::vector<std::thread> threads(num_threads - 1);
    Iterator block_start = first;
    for(unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(f, block_start, block_end);
        block_start = block_end;
    }
    f(block_start, last);
    for(auto& entry : threads)
        entry.join();
}
