#include "parallel_header.hpp"

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
