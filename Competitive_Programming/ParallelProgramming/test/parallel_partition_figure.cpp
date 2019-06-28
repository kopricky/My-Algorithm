#include <bits/stdc++.h>

template<typename RandomAccessIterator, class UnaryPredicate>
class ParallelPartitionSolver
{
private:
    const RandomAccessIterator first, last;
    const UnaryPredicate func;
    const unsigned length;
    const unsigned THRESHOLD = 250000;

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
        bool get_block(const unsigned block, int& left, int& right)
        {
            unsigned value = counter.fetch_add(block, std::memory_order_acq_rel) + block;
            left = (value >> 16);
            right = (value & lowWordMask);
            return left-- + right-- <= (int)block_count;
        }
    };

    BlockCounter counter;

    enum InPlace {Left, Right, Both};

    auto arrange_blocks
        (RandomAccessIterator& leftFrom, const RandomAccessIterator leftTo, RandomAccessIterator& rightFrom, const RandomAccessIterator rightTo);

    class ChunkResult
    {
    public:
        int leftRemaining, rightRemaining, leftMostBlocks, rightMostBlocks;
        ChunkResult() : leftRemaining(std::numeric_limits<int>::max()), rightRemaining(std::numeric_limits<int>::max())
            , leftMostBlocks(-1), rightMostBlocks(-1){}
    };

    std::vector<ChunkResult> remaining;

    auto arrange_chunk(const unsigned index);

    inline void swapBlocks(RandomAccessIterator from, RandomAccessIterator to, const int blockSize);

    const int LeftArrangeRemainingBlocks();

    const int RightArrangeRemainingBlocks();

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
    while(leftFrom < leftTo && rightFrom < rightTo){
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
    InPlace result = Both;
    while(true){
        if(result == Left || result == Both){
            localLeftMost = leftBlock;
            if(!counter.get_left_block(leftBlock))
                break;
            leftFrom = first + leftBlock * block_size;
            leftTo = leftFrom + block_size;
        }
        if(result == Right || result == Both){
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
        info.leftRemaining = leftBlock;
    if(rightFrom != rightTo)
        info.rightRemaining = rightBlock;
    info.leftMostBlocks = localLeftMost, info.rightMostBlocks = localRightMost;
    return info;
}

template<typename RandomAccessIterator, class UnaryPredicate>
inline void ParallelPartitionSolver<RandomAccessIterator, UnaryPredicate>::swapBlocks
    (RandomAccessIterator from, RandomAccessIterator to, const int blockSize)
{
    for(int i = 0; i < blockSize; ++i, ++from, ++to){
        std::iter_swap(from, to);
    }
}

template<typename RandomAccessIterator, class UnaryPredicate>
const int ParallelPartitionSolver<RandomAccessIterator, UnaryPredicate>::LeftArrangeRemainingBlocks()
{
    const int blockSize = counter.block_size;
    int j = -1;
    int leftmostBlock = std::max_element(remaining.begin(), remaining.end(), [](const ChunkResult& arg1, const ChunkResult& arg2)
    {
        return arg1.leftMostBlocks < arg2.leftMostBlocks;
    })->leftMostBlocks;
    std::sort(remaining.begin(), remaining.end(), [](const ChunkResult& arg1, const ChunkResult& arg2)
    {
        return arg1.leftRemaining < arg2.leftRemaining;
    });
    for(unsigned i = 0; i < remaining.size(); ++i)
        if(remaining[i].leftRemaining < leftmostBlock)
            j = i;
    for(unsigned i = 0; i < remaining.size() && remaining[i].leftRemaining <= leftmostBlock;){
        if(remaining[j].leftRemaining == leftmostBlock)
        {
            --j;
        }
        else
        {
            swapBlocks(first + remaining[i].leftRemaining * blockSize, first + leftmostBlock * blockSize, blockSize);
            ++i;
        }
        --leftmostBlock;
    }
    return leftmostBlock;
}

template<typename RandomAccessIterator, class UnaryPredicate>
const int ParallelPartitionSolver<RandomAccessIterator, UnaryPredicate>::RightArrangeRemainingBlocks()
{
    const int blockSize = counter.block_size;
    int j = -1;
    int rightmostBlock = std::max_element(remaining.begin(), remaining.end(), [](const ChunkResult& arg1, const ChunkResult& arg2)
    {
        return arg1.rightMostBlocks < arg2.rightMostBlocks;
    })->rightMostBlocks;
    std::sort(remaining.begin(), remaining.end(), [](const ChunkResult& arg1, const ChunkResult& arg2)
    {
        return arg1.rightRemaining < arg2.rightRemaining;
    });
    for(unsigned i = 0; i < remaining.size(); ++i)
        if(remaining[i].rightRemaining < rightmostBlock)
            j = i;
    for(unsigned i = 0; i < remaining.size() && remaining[i].rightRemaining <= rightmostBlock;)
    {
        if(remaining[j].rightRemaining == rightmostBlock)
        {
            --j;
        }
        else
        {
            swapBlocks(last - (remaining[i].rightRemaining + 1) * blockSize, last - (rightmostBlock + 1) * blockSize, blockSize);
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
    unsigned workerCount = std::min(std::thread::hardware_concurrency(), block_count / 2);
    remaining.resize(workerCount);
    std::vector<std::future<ChunkResult> > fut;
    for(unsigned i = 0; i < workerCount; ++i){
        fut.push_back(std::async(&ParallelPartitionSolver::arrange_chunk, this, i));
    }
    for(unsigned i = 0; i < workerCount; ++i){
        remaining[i] = fut[i].get();
    }
    const int leftMostBlock = LeftArrangeRemainingBlocks();
    const int rightMostBlock = RightArrangeRemainingBlocks();
    return std::partition(first + (leftMostBlock + 1) * block_size,
                            last - (rightMostBlock + 1) * block_size,
                                func);
}

template<typename RandomAccessIterator, class UnaryPredicate>
RandomAccessIterator parallel_partition
    (const RandomAccessIterator first, const RandomAccessIterator last, const UnaryPredicate func, const unsigned _THRESHOLD)
{
    ParallelPartitionSolver<RandomAccessIterator, UnaryPredicate> solver(first, last, func, _THRESHOLD);
    return solver();
}

using namespace std;

struct SW {
    chrono::high_resolution_clock::time_point st,ed;
    void start() {
        st = chrono::high_resolution_clock::now();
    }
    int end() {
        ed = chrono::high_resolution_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(ed - st).count();
    }
};

#include "matplotlib.hpp"
namespace plt = matplotlibcpp;

template<typename T>
void print(vector<T>& x)
{
    vector<T> t((int)x.size());
    iota(t.begin(), t.end(), 0);
    plt::plot(t,x), plt::show();
}

template<typename T>
void print(vector<T>& x, vector<T>& y)
{
    plt::plot(x,y), plt::show();
}

int main()
{
    int n = 100000000;
    vector<int> vec1(n), vec2;
    iota(vec1.begin(), vec1.end(), 0);
    shuffle(vec1.begin(), vec1.end(), mt19937(random_device{}()));
    // for(int val : vec1){
    //     cout << val << " ";
    // }
    // cout << endl;
    // shuffle(vec1.begin(), vec1.end(), mt19937(random_device{}()));
    // vec2 = vec1;
    SW sw1, sw2;
    vector<int> cand = {100, 300, 500, 700, 900, 1500, 2000, 4000, 8000, 10000};
    vector<int> res;
    for(int val : cand){
        vec2 = vec1;
        sw1.start();
        int pivot = vec2[0];
        parallel_partition(vec2.begin(), vec2.end(), [pivot](const auto& itr){ return itr < pivot; }, val);
        res.push_back(sw1.end());
    }
    print(cand, res);
    // sw2.start();
    // int pivot = vec2[0];
    // int res2 = std::partition(vec2.begin(), vec2.end(), [pivot](const int& itr){ return itr < pivot; }) - vec2.begin();
    // sw2.end();
    // assert(res1 == res2);
}
