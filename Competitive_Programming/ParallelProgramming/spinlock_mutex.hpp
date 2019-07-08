#include "parallel_header.hpp"

class SpinlockMutex
{
    std::atomic_flag flag;
public:
    SpinlockMutex() : flag(ATOMIC_FLAG_INIT){}
    void lock()
    {
        while(flag.test_and_set(std::memory_order_acquire));
    }
    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
};
