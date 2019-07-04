#include "parallel_header.hpp"

template<typename RandomAccessIterator, class UnaryPredicate>
class ParallelPartitionSolver
{
private:
    const RandomAccessIterator first, last;
    const UnaryPredicate func;
    const unsigned length;
    static const unsigned THRESHOLD = 8192;
    enum Process {Left, Right, Both};

    class BlockCounter
    {
    private:
        static const unsigned leftBlock = 1 << 16;
        static const unsigned rightBlock = 1;
        static const unsigned mask = leftBlock - 1;
        static const unsigned min_blockSize = 1024;
        static const unsigned max_blockCount = (1 << 15) - 1;

    public:
        unsigned block_size, block_count;
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
        (const RandomAccessIterator _first, const RandomAccessIterator _last, const UnaryPredicate _func)
        : first(_first), last(_last), func(_func), length(std::distance(_first, _last)), counter(length){}
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
    Process result = Both;
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
    unsigned workerCount = std::min(std::thread::hardware_concurrency(), block_count / 2);
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
    (const RandomAccessIterator first, const RandomAccessIterator last, const UnaryPredicate func)
{
    ParallelPartitionSolver<RandomAccessIterator, UnaryPredicate> solver(first, last, func);
    return solver();
}
