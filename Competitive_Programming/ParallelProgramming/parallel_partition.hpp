#include "parallel_header.hpp"

enum InPlace {Left, Right, Both};

InPlace ArrangeBlocks(int pivot, int leftFrom, int leftTo, int rightFrom, int rightTo)
{
    while(leftFrom < leftTo && rightFrom < rightTo){
        while(m_arr[leftFrom] <= pivot && ++leftFrom < leftTo){

        }
        while(m_arr[rightFrom] >= pivot && ++rightFrom < rightTo){

        }
        if(leftFrom == leftTo || rightFrom == rightTo)
            break;
        std::swap(++leftFrom, ++rightFrom);
    }
    if(leftFrom == leftTo && rightFrom == rightTo)
        return Both;
    if(leftFrom == leftTo)
        return Left;
    return Right;
}

class BlockCounter
{
public:
    int size, count;
    int c_leftBlock = 1 << 16;
    int c_rightBlock = 1;
    int c_lowWordMask = 0x0000FFFF;
    BlockCounter(int size)
    {

    }
    bool TakeLeftBlock(int left)
    {
        int ignore;
        return TakeBlock(c_leftBlock, left, ignore);
    }
    bool TakeRightBlock(int left)
    {
        int ignore;
        return TakeBlock(c_rightBlock, left, ignore);
    }
    bool TakeBlock(int block, int left, int right)
    {
        int
    }
};

int ParallelPartition(int pivot, int from, int to)
{
    int size = to - from;
    BlockCounter counter(size);
    int blockCount = counter.count;
    int blockSize = counter.size;
    int workercount = std::min((int)std::thread::hardware_concurrency(), blockCount / 2);
    int leftRemaining = AllocateRemainingArray(workerCount);
    int rightRemaining = AllocateRemainingArray(workerCount);
    int leftMostBlocks = AllocateMostArray(workerCount);
    int rightMostBlocks = AllocateMostArray(workerCount);
    {
        int localLeftMost = -1, localRightMost = -1;
        int leftBlock = localLeftMost;
        int rightBlock = localRightMost;
        int leftFrom = 0, leftTo = 0;
        int rightFrom = 0, rightTo = 0;
        int result = InPlace.Both;
        while(true){
            if(result == Left || result == Both){
                localLeftMost = leftBlock;
                if(!counter.)
            }
        }
    }
}
