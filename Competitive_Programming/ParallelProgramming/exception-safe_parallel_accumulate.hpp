#include "parallel_header.hpp"

template<typename ForwardIterator, typename T, class AssociativeBinaryOperation>
struct accumulate_block
{
    T operator()(const ForwardIterator first, const ForwardIterator last, const AssociativeBinaryOperation op)
    {
        return std::accumulate(first, last, T(), op);
    }
};

template<typename ForwardIterator, typename T, class AssociativeBinaryOperation=std::plus<T> >
T parallel_accumulate(const ForwardIterator first, const ForwardIterator last, T init, const AssociativeBinaryOperation op=AssociativeBinaryOperation())
{
    const unsigned long length = std::distance(first, last);
    if(!length)
        return init;
    const unsigned long min_per_thread = 25;
    const unsigned long max_threads = (length+min_per_thread - 1) / min_per_thread;
    const unsigned long hardware_threads = std::thread:: hardware_concurrency();
    const unsigned long num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    const unsigned long block_size = length / num_threads;
    std::vector<std::future<T> > futures(num_threads - 1);
    std::vector<std::thread> threads(num_threads - 1);
    ForwardIterator block_start = first;
    for(unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        ForwardIterator block_end = block_start;
        std::advance(block_end, block_size);
        std::packaged_task<T(ForwardIterator, ForwardIterator, AssociativeBinaryOperation)> task([](
        const ForwardIterator first, const ForwardIterator last, const AssociativeBinaryOperation op)
        {
            return std::accumulate(first, last, T(), op);
        });
        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task), block_start, block_end, op);
        block_start = block_end;
    }
    T last_result = std::accumulate(block_start, last, T(), op);
    // mem_fn でメンバ関数を呼び出すオブジェクトを生成する.
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    T result = init;
    for(unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        result += futures[i].get();
    }
    result += last_result;
    return result;
}
