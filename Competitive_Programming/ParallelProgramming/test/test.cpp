#include <bits/stdc++.h>

class BlockCounter
{
private:
    static const unsigned leftBlock = 1 << 16;
    static const unsigned rightBlock = 1;
    static const unsigned lowWordMask = leftBlock - 1;
    static const unsigned minBlockSize = 1024;
    static const unsigned maxBlockCount = (1 << 15) - 1;

public:
    unsigned block_size, block_count;
    std::atomic<unsigned> counter;
    BlockCounter(unsigned length)
        : block_size(std::max((length - 1) / maxBlockCount + 1, minBlockSize)),
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
    bool get_block(unsigned block, int& left, int& right)
    {
        unsigned value = counter.fetch_add(block, std::memory_order_acq_rel) + block;
        left = (value >> 16);
        right = (value & lowWordMask);
        return left-- + right-- <= (int)block_count;
    }
};

enum InPlace {Left, Right, Both};

template<typename Iterator, typename T>
InPlace arrange_blocks(Iterator& leftFrom, Iterator leftTo, Iterator& rightFrom, Iterator rightTo, T pivot)
{
    while(leftFrom < leftTo && rightFrom < rightTo){
        while(*leftFrom < pivot && ++leftFrom < leftTo);
        while(*rightFrom >= pivot && ++rightFrom < rightTo);
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

class ChunkResult
{
public:
    int leftRemaining, rightRemaining, leftMostBlocks, rightMostBlocks;
    ChunkResult() : leftRemaining(0), rightRemaining(0), leftMostBlocks(0), rightMostBlocks(0){}
};

template<typename Iterator, typename T>
ChunkResult arrange_chunk
    (const Iterator from, const Iterator to, T pivot, BlockCounter& counter, unsigned index)
{
    int localLeftMost = -1, localRightMost = -1;
    int leftBlock = localLeftMost, rightBlock = localRightMost;
    Iterator leftFrom, rightFrom;
    Iterator leftTo = leftFrom, rightTo = rightFrom;
    unsigned block_size = counter.block_size;
    InPlace result = Both;
    while(true){
        if(result == Left || result == Both){
            localLeftMost = leftBlock;
            if(!counter.get_left_block(leftBlock))
                break;
            leftFrom = from + leftBlock * block_size;
            leftTo = leftFrom + block_size;
        }
        if(result == Right || result == Both){
            localRightMost = rightBlock;
            if(!counter.get_right_block(rightBlock))
                break;
            rightTo = to - rightBlock * block_size;
            rightFrom = rightTo - block_size;
        }
        result = arrange_blocks(leftFrom, leftTo, rightFrom, rightTo, pivot);
    }
    ChunkResult info;
    if(leftFrom != leftTo)
        info.leftRemaining = leftBlock;
    if(rightFrom != rightTo)
        info.rightRemaining = rightBlock;
    info.leftMostBlocks = localLeftMost, info.rightMostBlocks = localRightMost;
    return info;
}

template<typename Iterator>
void swapBlocks(Iterator from, Iterator to, const int blockSize)
{
    for(int i = 0; i < blockSize; ++i, ++from, ++to){
        std::iter_swap(from, to);
    }
}

template<typename Iterator>
std::pair<int, int> ArrangeRemainingBlocks(const Iterator from, const Iterator to, int blockSize, std::vector<ChunkResult> remaining)
{
    int j = -1, k = -1;
    int leftmostBlock = std::max_element(remaining.begin(), remaining.end(), [](const ChunkResult& arg1, const ChunkResult& arg2){
        return arg1.leftMostBlocks < arg2.leftMostBlocks;
    })->leftMostBlocks;
    int rightmostBlock = std::max_element(remaining.begin(), remaining.end(), [](const ChunkResult& arg1, const ChunkResult& arg2){
        return arg1.rightMostBlocks < arg2.rightMostBlocks;
    })->rightMostBlocks;
    std::sort(remaining.begin(), remaining.end(), [](const ChunkResult& arg1, const ChunkResult& arg2){
        return arg1.leftRemaining < arg2.leftRemaining;
    });
    for(unsigned i = 0; i < remaining.size(); ++i){
        if(remaining[i].leftRemaining < leftmostBlock)
            j = i;
    }
    // i, j が前, 後ろから近づいていく感じ
    for(unsigned i = 0; i < remaining.size() && remaining[i].leftRemaining <= leftmostBlock;){
        if(remaining[j].leftRemaining == leftmostBlock){
            --j;
        }else{
            // remaining[i].leftRemaining 番目のブロックの要素(incomplete)と leftmostBlock 番目のブロックの要素(complete)を swap
            swapBlocks(from + remaining[i].leftRemaining * blockSize, from + leftmostBlock * blockSize, blockSize);
            ++i;
        }
        // leftmostBlock が消費された
        --leftmostBlock;
    }
    std::sort(remaining.begin(), remaining.end(), [](const ChunkResult& arg1, const ChunkResult& arg2){
        return arg1.rightRemaining < arg2.rightRemaining;
    });
    for(unsigned i = 0; i < remaining.size(); ++i){
        if(remaining[i].rightRemaining < rightmostBlock)
            k = i;
    }
    for(unsigned i = 0; i < remaining.size() && remaining[i].rightRemaining <= rightmostBlock;){
        if(remaining[k].rightRemaining == rightmostBlock){
            --k;
        }else{
            swapBlocks(to - (remaining[i].rightRemaining + 1) * blockSize, to - (rightmostBlock + 1) * blockSize, blockSize);
            ++i;
        }
        --rightmostBlock;
    }
    return std::make_pair(leftmostBlock, rightmostBlock);
}

template<typename Iterator, typename T>
Iterator parallel_partition(const Iterator first, const Iterator last, T pivot)
{
    unsigned length = std::distance(first, last);
    const unsigned THRESHOULD = 2500000;
    if(length <= THRESHOULD)
        return std::partition(first, last, [pivot](const auto& itr){ return itr < pivot; });
    BlockCounter counter(length);
    const unsigned block_count = counter.block_count;
    const unsigned block_size = counter.block_size;
    unsigned workerCount = std::min(std::thread::hardware_concurrency(), block_count / 2);
    std::vector<ChunkResult> result(workerCount);
    std::vector<std::future<ChunkResult> > fut;
    for(unsigned i = 0; i < workerCount-1; ++i){
        fut.push_back(std::async(arrange_chunk<Iterator, T>, first, last, pivot, std::ref(counter), i));
    }
    result[workerCount-1] = arrange_chunk(first, last, pivot, counter, workerCount-1);
    for(unsigned i = 0; i < workerCount-1; ++i){
        result[i] = fut[i].get();
    }
    std::pair<int, int> MostBlock = ArrangeRemainingBlocks<Iterator>(first, last, block_size, result);
    return std::partition(first + (MostBlock.first + 1) * block_size,
                            last - (MostBlock.second + 1) * block_size,
                                [pivot](const auto& itr){ return itr < pivot; });
}

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
        num.fetch_add(1, std::memory_order_acq_rel);
        value = data_queue.front();
        data_queue.pop();
        return true;
    }
};

template<typename Iterator>
class ParallelQuickSortSolver {
private:
    threadsafe_queue<std::pair<Iterator, Iterator> > task_queue;
    std::mt19937 mt;
    std::atomic<unsigned> actthread_num;
    std::atomic<bool> finish;
    static const unsigned THRESHOLD = 400000;
    void parallel_partial_sort(const Iterator first, const Iterator last);

public:
    ParallelQuickSortSolver(const Iterator first, const Iterator last)
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
void ParallelQuickSortSolver<Iterator>::parallel_partial_sort(const Iterator first, const Iterator last)
{
    const unsigned length = std::distance(first, last);
    if(length <= THRESHOLD)
    {
        if(length >= 2)
            std::sort(first, last);
        actthread_num.fetch_sub(1, std::memory_order_acq_rel);
        return;
    }
    std::uniform_int_distribution<> uid(0, length);
    auto pivot = *std::next(first, uid(mt));
    Iterator middle1 = parallel_partition(first, last, pivot);
    // Iterator middle2 = std::partition(first, last, [pivot](const auto& itr){ return !(pivot < itr); });
    task_queue.push(std::make_pair(first, middle1));
    parallel_partial_sort(middle1, last);
}

template<typename Iterator>
void parallel_quick_sort(Iterator first, Iterator last)
{
    ParallelQuickSortSolver<Iterator> solver(first, last);
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

int main()
{

}
