#include "parallel_header.hpp"

template<typename T>
class threadsafe_queue
{
private:
    std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue(){}
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        bool call_flag = data_queue.empty();
        data_queue.push(new_value);
        if(call_flag)
            data_cond.notify_one();
    }
    bool wait_and_pop(T& value, std::atomic<unsigned>& num, std::atomic<bool>& state)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [&]
        {
            return !data_queue.empty() || (num.load(std::memory_order_acquire) == 0);
        });
        if(data_queue.empty())
        {
            if(!state.load(std::memory_order_acquire))
            {
                state.store(true, std::memory_order_release);
                data_cond.notify_all();
            }
            return false;
        }
        num.fetch_add(1, std::memory_order_release);
        value = data_queue.front();
        data_queue.pop();
        return true;
    }
};

template<typename Iterator>
class Solver {
private:
    threadsafe_queue<std::pair<Iterator, Iterator> > task_queue;
    std::mt19937 mt;
    std::atomic<unsigned> actthread_num;
    std::atomic<bool> finish;
    static const unsigned THRESHOLD = 15000;
    void parallel_partial_sort(const Iterator first, const Iterator last);

public:
    Solver(const Iterator first, const Iterator last)
        : task_queue(), mt(std::random_device{}()), actthread_num(0), finish(false)
    {
        task_queue.push(std::make_pair(first, last));
    }
    void operator()()
    {
        std::pair<Iterator, Iterator> task;
        while(finish.load(std::memory_order_acquire) || task_queue.wait_and_pop(task, actthread_num, finish))
        {
            parallel_partial_sort(task.first, task.second);
        }
    }
};

template<typename Iterator>
void Solver<Iterator>::parallel_partial_sort(const Iterator first, const Iterator last)
{
    const unsigned length = std::distance(first, last);
    if(length <= THRESHOLD)
    {
        if(length >= 2)
            std::sort(first, last);
        actthread_num.fetch_sub(1, std::memory_order_release);
        return;
    }
    std::uniform_int_distribution<> uid(0, length);
    auto pivot = *std::next(first, uid(mt));
    Iterator middle1 = std::partition(first, last, [pivot](const auto& itr){ return itr < pivot; });
    Iterator middle2 = std::partition(first, last, [pivot](const auto& itr){ return !(pivot < itr); });
    task_queue.push(std::make_pair(first, middle1));
    parallel_partial_sort(middle2, last);
}

template<typename Iterator>
void parallel_quick_sort(Iterator first, Iterator last)
{
    Solver<Iterator> solver(first, last);
    const unsigned hardware_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    for(unsigned i = 0; i < hardware_threads-1; ++i)
    {
        threads.push_back(std::thread(std::ref(solver)));
    }
    solver();
    for(unsigned i = 0; i < hardware_threads-1; ++i)
    {
        threads[i].join();
    }
}
