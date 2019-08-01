#include "parallel_header.hpp"

template<typename RandomAccessIterator, class UnaryPredicate>
class ParallelPartitionSolver
{
private:
    const RandomAccessIterator first, last;
    const UnaryPredicate func;
    const unsigned length;
    const unsigned THRESHOLD;
    enum Update {Left, Right, Both};

    class BlockCounter
    {
    private:
        static const unsigned leftBlock = 1 << 16;
        static const unsigned rightBlock = 1;
        static const unsigned mask = leftBlock - 1;
        static const unsigned min_blockSize = 1024;
        static const unsigned max_blockCount = (1 << 15) - 1;

    public:
        const unsigned block_size, block_count;
        std::atomic<unsigned> counter;
        BlockCounter(unsigned length)
            : block_size(std::max((length - 1) / max_blockCount + 1, min_blockSize)),
                block_count(length / block_size), counter(0){}
        bool get_left_block(int& left)
        {
            int ignore;
            return get_block(leftBlock, left, ignore);
        }
        bool get_right_block(int& right)
        {
            int ignore;
            return get_block(rightBlock, ignore, right);
        }
        bool get_block(const unsigned block, int& left, int& right)
        {
            const unsigned value = counter.fetch_add(block, std::memory_order_acq_rel) + block;
            left = (value >> 16);
            right = (value & mask);
            return left-- + right-- <= (int)block_count;
        }
    };

    class ChunkResult
    {
    public:
        int left_remain, right_remain, leftMost_blocks, rightMost_blocks;
        ChunkResult() : left_remain(std::numeric_limits<int>::max()), right_remain(std::numeric_limits<int>::max())
            , leftMost_blocks(-1), rightMost_blocks(-1){}
    };

    BlockCounter counter;
    std::vector<ChunkResult> remain;

    auto arrange_blocks
        (RandomAccessIterator& leftFrom, const RandomAccessIterator leftTo, RandomAccessIterator& rightFrom, const RandomAccessIterator rightTo);
    auto arrange_chunk(const unsigned index);
    inline void swapBlocks(RandomAccessIterator from, RandomAccessIterator to, const int blockSize);
    const int arrange_leftBlocks();
    const int arrange_rightBlocks();

public:
    ParallelPartitionSolver
        (const RandomAccessIterator _first, const RandomAccessIterator _last, const UnaryPredicate _func, const unsigned _THRESHOLD)
        : first(_first), last(_last), func(_func), length(std::distance(_first, _last)), THRESHOLD(_THRESHOLD), counter(length){}
    RandomAccessIterator operator()();
};

template<typename RandomAccessIterator, class UnaryPredicate>
auto ParallelPartitionSolver<RandomAccessIterator, UnaryPredicate>::arrange_blocks
    (RandomAccessIterator& leftFrom, const RandomAccessIterator leftTo, RandomAccessIterator& rightFrom, const RandomAccessIterator rightTo)
{
    while(leftFrom < leftTo && rightFrom < rightTo)
    {
        while(func(*leftFrom) && ++leftFrom < leftTo);
        while(!func(*rightFrom) && ++rightFrom < rightTo);
        if(leftFrom == leftTo || rightFrom == rightTo)
            break;
        std::iter_swap(leftFrom, rightFrom);
        ++leftFrom, ++rightFrom;
    }
    if(leftFrom == leftTo && rightFrom == rightTo)
        return Both;
    if(leftFrom == leftTo)
        return Left;
    return Right;
}

template<typename RandomAccessIterator, class UnaryPredicate>
auto ParallelPartitionSolver<RandomAccessIterator, UnaryPredicate>::arrange_chunk(const unsigned index)
{
    int localLeftMost = -1, localRightMost = -1;
    int leftBlock = localLeftMost, rightBlock = localRightMost;
    RandomAccessIterator leftFrom, rightFrom;
    RandomAccessIterator leftTo = leftFrom, rightTo = rightFrom;
    unsigned block_size = counter.block_size;
    Update result = Both;
    while(true)
    {
        if(result == Left || result == Both)
        {
            localLeftMost = leftBlock;
            if(!counter.get_left_block(leftBlock))
                break;
            leftFrom = first + leftBlock * block_size;
            leftTo = leftFrom + block_size;
        }
        if(result == Right || result == Both)
        {
            localRightMost = rightBlock;
            if(!counter.get_right_block(rightBlock))
                break;
            rightTo = last - rightBlock * block_size;
            rightFrom = rightTo - block_size;
        }
        result = arrange_blocks(leftFrom, leftTo, rightFrom, rightTo);
    }
    ChunkResult info;
    if(leftFrom != leftTo)
        info.left_remain = leftBlock;
    if(rightFrom != rightTo)
        info.right_remain = rightBlock;
    info.leftMost_blocks = localLeftMost, info.rightMost_blocks = localRightMost;
    return info;
}

template<typename RandomAccessIterator, class UnaryPredicate>
inline void ParallelPartitionSolver<RandomAccessIterator, UnaryPredicate>::swapBlocks
    (RandomAccessIterator from, RandomAccessIterator to, const int blockSize)
{
    for(int i = 0; i < blockSize; ++i, ++from, ++to)
    {
        std::iter_swap(from, to);
    }
}

template<typename RandomAccessIterator, class UnaryPredicate>
const int ParallelPartitionSolver<RandomAccessIterator, UnaryPredicate>::arrange_leftBlocks()
{
    const int blockSize = counter.block_size;
    int j = -1;
    int leftmostBlock = std::max_element(remain.begin(), remain.end(), [](const ChunkResult& arg1, const ChunkResult& arg2)
    {
        return arg1.leftMost_blocks < arg2.leftMost_blocks;
    })->leftMost_blocks;
    std::sort(remain.begin(), remain.end(), [](const ChunkResult& arg1, const ChunkResult& arg2)
    {
        return arg1.left_remain < arg2.left_remain;
    });
    for(unsigned i = 0; i < remain.size(); ++i)
        if(remain[i].left_remain < leftmostBlock)
            j = i;
    for(unsigned i = 0; i < remain.size() && remain[i].left_remain <= leftmostBlock;){
        if(remain[j].left_remain == leftmostBlock)
        {
            --j;
        }
        else
        {
            swapBlocks(first + remain[i].left_remain * blockSize, first + leftmostBlock * blockSize, blockSize);
            ++i;
        }
        --leftmostBlock;
    }
    return leftmostBlock;
}

template<typename RandomAccessIterator, class UnaryPredicate>
const int ParallelPartitionSolver<RandomAccessIterator, UnaryPredicate>::arrange_rightBlocks()
{
    const int blockSize = counter.block_size;
    int j = -1;
    int rightmostBlock = std::max_element(remain.begin(), remain.end(), [](const ChunkResult& arg1, const ChunkResult& arg2)
    {
        return arg1.rightMost_blocks < arg2.rightMost_blocks;
    })->rightMost_blocks;
    std::sort(remain.begin(), remain.end(), [](const ChunkResult& arg1, const ChunkResult& arg2)
    {
        return arg1.right_remain < arg2.right_remain;
    });
    for(unsigned i = 0; i < remain.size(); ++i)
        if(remain[i].right_remain < rightmostBlock)
            j = i;
    for(unsigned i = 0; i < remain.size() && remain[i].right_remain <= rightmostBlock;)
    {
        if(remain[j].right_remain == rightmostBlock)
        {
            --j;
        }
        else
        {
            swapBlocks(last - (remain[i].right_remain + 1) * blockSize, last - (rightmostBlock + 1) * blockSize, blockSize);
            ++i;
        }
        --rightmostBlock;
    }
    return rightmostBlock;
}

template<typename RandomAccessIterator, class UnaryPredicate>
RandomAccessIterator ParallelPartitionSolver<RandomAccessIterator, UnaryPredicate>::operator()()
{
    if(length <= THRESHOLD)
        return std::partition(first, last, func);
    const unsigned block_count = counter.block_count;
    const unsigned block_size = counter.block_size;
    const unsigned workerCount = std::min(std::thread::hardware_concurrency(), block_count / 2);
    std::vector<std::future<ChunkResult> > fut;
    for(unsigned i = 0; i < workerCount; ++i)
    {
        fut.push_back(std::async(&ParallelPartitionSolver::arrange_chunk, this, i));
    }
    for(auto& entry : fut)
    {
        remain.push_back(entry.get());
    }
    const int leftMostBlock = arrange_leftBlocks();
    const int rightMostBlock = arrange_rightBlocks();
    return std::partition(first + (leftMostBlock + 1) * block_size,
                            last - (rightMostBlock + 1) * block_size, func);
}

template<typename RandomAccessIterator, class UnaryPredicate>
RandomAccessIterator parallel_partition
    (const RandomAccessIterator first, const RandomAccessIterator last, const UnaryPredicate func, const unsigned THRESHOLD)
{
    ParallelPartitionSolver<RandomAccessIterator, UnaryPredicate> solver(first, last, func, THRESHOLD);
    return solver();
}

template<typename T>
class ThreadSafeQueue
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
    void pop_head()
    {
        std::unique_ptr<Node> old_head = std::move(head);
        head = std::move(old_head->next);
    }

public:
    ThreadSafeQueue() : head(new Node), tail(head.get()){}
    void push(const T new_value)
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
    bool wait_and_pop(T& value, std::atomic<unsigned>& num, std::atomic<bool>& state)
    {
        std::unique_lock<std::mutex> head_lock(head_mutex);
        bool check;
        data_cond.wait(head_lock, [&]
        {
            return (check = (head.get() != get_tail())) || (num.load(std::memory_order_acquire) == 0);
        });
        if(!check)
        {
            if(!state.load(std::memory_order_acquire))
            {
                state.store(true, std::memory_order_release);
                data_cond.notify_all();
            }
            return false;
        }
        num.fetch_add(1, std::memory_order_acq_rel);
        value = std::move(*head->data);
        pop_head();
        return true;
    }
};

template<typename RandomAccessIterator, class Compare>
class ParallelQuickSortSolver {
private:
    const RandomAccessIterator first, last;
    const Compare comp;
    ThreadSafeQueue<std::pair<RandomAccessIterator, RandomAccessIterator> > task_queue;
    std::mt19937 mt;
    std::atomic<unsigned> actthread_num;
    std::atomic<bool> finish;
    const unsigned total_length;
    const unsigned THRESHOLD;

    void getTask_and_run();
    void parallel_partial_sort(const RandomAccessIterator _first, const RandomAccessIterator _last);

public:
    ParallelQuickSortSolver(const RandomAccessIterator _first, const RandomAccessIterator _last, const Compare _comp)
        : first(_first), last(_last), comp(_comp), task_queue(), mt(std::random_device{}()),
            actthread_num(0), finish(false), total_length(std::distance(first, last)), THRESHOLD(total_length / 200)
    {
        task_queue.push(std::make_pair(first, last));
    }
    void operator()();
};

template<typename RandomAccessIterator, class Compare>
void ParallelQuickSortSolver<RandomAccessIterator, Compare>::getTask_and_run()
{
    std::pair<RandomAccessIterator, RandomAccessIterator> task;
    while(finish.load(std::memory_order_acquire) || task_queue.wait_and_pop(task, actthread_num, finish))
    {
        parallel_partial_sort(task.first, task.second);
    }
}

template<typename RandomAccessIterator, class Compare>
void ParallelQuickSortSolver<RandomAccessIterator, Compare>::parallel_partial_sort(const RandomAccessIterator _first, const RandomAccessIterator _last)
{
    const unsigned length = std::distance(_first, _last);
    if(length <= THRESHOLD)
    {
        if(length >= 2)
            std::sort(_first, _last);
        actthread_num.fetch_sub(1, std::memory_order_acq_rel);
        return;
    }
    std::uniform_int_distribution<> uid(0, length-1);
    const RandomAccessIterator pivot_itr = std::next(_first, uid(mt));
    const auto pivot = *pivot_itr;
    std::iter_swap(_first, pivot_itr);
    const RandomAccessIterator middle = parallel_partition(_first + 1, _last, std::bind(comp, std::placeholders::_1, std::cref(pivot)), total_length / 4);
    std::iter_swap(_first, middle - 1);
    task_queue.push(std::make_pair(_first, middle - 1));
    parallel_partial_sort(middle, _last);
}

template<typename RandomAccessIterator, class Compare>
void ParallelQuickSortSolver<RandomAccessIterator, Compare>::operator()()
{
    const unsigned hardware_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    for(unsigned i = 0; i < hardware_threads-1; ++i)
    {
        threads.push_back(std::thread(&ParallelQuickSortSolver<RandomAccessIterator, Compare>::getTask_and_run, this));
    }
    getTask_and_run();
    for(auto& entry : threads)
    {
        entry.join();
    }
}

template<typename RandomAccessIterator, class Compare=std::less<typename RandomAccessIterator::value_type> >
void parallel_quick_sort(const RandomAccessIterator first, const RandomAccessIterator last, const Compare comp=Compare())
{
    ParallelQuickSortSolver<RandomAccessIterator, Compare> solver(first, last, comp);
    solver();
}
