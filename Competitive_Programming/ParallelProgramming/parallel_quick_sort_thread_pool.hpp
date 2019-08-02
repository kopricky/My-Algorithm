#include "parallel_header.hpp"

// fut の while 文のパートが遅い
template<typename T>
class ThreadsafeQueue
{
private:
    class Node
    {
    public:
        std::shared_ptr<T> data;
        std::unique_ptr<Node> next;
    };
    std::mutex head_mutex;
    std::unique_ptr<Node> head;
    std::mutex tail_mutex;
    Node* tail;
    std::condition_variable data_cond;

    Node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }
    std::unique_ptr<Node> pop_head()
    {
        std::unique_ptr<Node> old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }
    std::unique_lock<std::mutex> wait_for_data()
    {
        std::unique_lock<std::mutex> head_lock(head_mutex);
        data_cond.wait(head_lock, [&]
        {
            return head.get() != get_tail();
        });
        return std::move(head_lock);
    }
    std::unique_ptr<Node> try_pop_head()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if(head.get() == get_tail())
        {
            return std::unique_ptr<Node>();
        }
        return pop_head();
    }
    std::unique_ptr<Node> try_pop_head(T& value)
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if(head.get() == get_tail())
        {
            return std::unique_ptr<Node>();
        }
        value = std::move(*head->data);
        return pop_head();
    }
    std::unique_ptr<Node> wait_pop_head()
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        return pop_head();
    }
    std::unique_ptr<Node> wait_pop_head(T& value)
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        value = std::move(*head->data);
        return pop_head();
    }

public:
    ThreadsafeQueue() : head(new Node), tail(head.get()){}
    ThreadsafeQueue(const ThreadsafeQueue& another) = delete;
    ThreadsafeQueue& operator=(const ThreadsafeQueue& another) = delete;
    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<Node> old_head = try_pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }
    bool try_pop(T& value)
    {
        const std::unique_ptr<Node> old_head= try_pop_head(value);
        return (bool)old_head;
    }
    std::shared_ptr<T> wait_and_pop()
    {
        const std::unique_ptr<Node> old_head = wait_pop_head();
        return old_head->data;
    }
    void wait_and_pop(T& value)
    {
        const std::unique_ptr<Node> old_head = wait_pop_head(value);
    }
    void push(T new_value)
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<Node> p(new Node);
        {
            std::lock_guard<std::mutex> tail_lock(tail_mutex);
            tail->data = new_data;
            Node* const new_tail = p.get();
            tail->next = std::move(p);
            tail = new_tail;
        }
        data_cond.notify_one();
    }
    bool empty()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        return (head.get() == get_tail());
    }
};

class JoinThreads
{
    std::vector<std::thread>& threads;
public:
    explicit JoinThreads(std::vector<std::thread>& _threads):
        threads(_threads){}
    ~JoinThreads()
    {
        for(unsigned long i = 0; i < threads.size(); ++i)
        {
            if(threads[i].joinable())
                threads[i].join();
        }
    }
};

class FunctionWrapper
{
    struct impl_base
    {
        virtual void call() = 0;
        virtual ~impl_base(){}
    };
    std::unique_ptr<impl_base> impl;
    template<typename F>
    struct impl_type : impl_base
    {
        F f;
        impl_type(F&& _f) : f(std::move(_f)){}
        void call()
        {
            f();
        }
    };
public:
    template<typename F>
    FunctionWrapper(F&& f) : impl(new impl_type<F>(std::move(f))){}
    void operator()()
    {
        impl->call();
    }
    FunctionWrapper() = default;
    FunctionWrapper(FunctionWrapper&& other) : impl(std::move(other.impl)){}
    FunctionWrapper& operator=(FunctionWrapper&& other)
    {
        impl = std::move(other.impl);
        return *this;
    }
    FunctionWrapper(const FunctionWrapper&) = delete;
    FunctionWrapper(FunctionWrapper&) = delete;
    FunctionWrapper& operator=(const FunctionWrapper&) = delete;
};

class ThreadPool
{
    std::atomic_bool done;
    ThreadsafeQueue<FunctionWrapper> work_queue;
    std::vector<std::thread> threads;
    JoinThreads joiner;
    void worker_thread()
    {
        while(!done)
        {
            run_pending_task();
        }
    }
public:
    ThreadPool() : done(false), joiner(threads)
    {
        const unsigned thread_count = std::thread::hardware_concurrency();
        try
        {
            for(unsigned i = 0; i < thread_count; ++i)
            {
                threads.push_back(std::thread(&ThreadPool::worker_thread, this));
            }
        }
        catch(...)
        {
            done = true;
            throw;
        }
    }
    ~ThreadPool()
    {
        done = true;
    }
    template<typename Function>
    std::future<typename std::result_of<Function()>::type> submit(Function f)
    {
        using result_type = typename std::result_of<Function()>::type;
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());
        work_queue.push(std::move(task));
        return res;
    }
    void run_pending_task()
    {
        FunctionWrapper task;
        if(work_queue.try_pop(task))
        {
            task();
        }
        else
        {
            std::this_thread::yield();
        }
    }
};

template<typename BidirectionalIterator, class Compare>
class ParallelQuickSortSolver {
private:
    const BidirectionalIterator first, last;
    const Compare comp;
    ThreadPool thread_pool;
    std::mt19937 mt;
    static const unsigned long THRESHOLD = (1u << 14);
    void parallel_partial_sort(const BidirectionalIterator _first, const BidirectionalIterator _last);

public:
    ParallelQuickSortSolver(const BidirectionalIterator _first, const BidirectionalIterator _last, const Compare _comp)
        : first(_first), last(_last), comp(_comp), thread_pool(), mt(std::random_device{}()){}
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
    std::uniform_int_distribution<> uid(0, length-1);
    const auto pivot_itr = std::next(_first, uid(mt));
    const auto pivot = *pivot_itr;
    std::iter_swap(_first, pivot_itr);
    const BidirectionalIterator middle = std::partition(_first + 1, _last, std::bind(comp, std::placeholders::_1, std::cref(pivot)));
    std::iter_swap(_first, middle - 1);
    auto fut = std::async(std::bind(&ParallelQuickSortSolver::parallel_partial_sort, this, _first, middle - 1));
    parallel_partial_sort(middle, _last);
    while(fut.wait_for(std::chrono::seconds(0)) == std::future_status::timeout)
    {
        thread_pool.run_pending_task();
    }
}

template<typename BidirectionalIterator, class Compare=std::less<typename BidirectionalIterator::value_type> >
void parallel_quick_sort(const BidirectionalIterator first, const BidirectionalIterator last, const Compare comp=Compare())
{
    ParallelQuickSortSolver<BidirectionalIterator, Compare> solver(first, last, comp);
    solver();
}
