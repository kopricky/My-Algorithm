#include "../parallel_stack.hpp"
#include <bits/stdc++.h>

using namespace std;

const int MAX_N = 100000000;

threadsafe_stack<int> st1;
stack<int> st2;
vector<int> ok;

template<class Function, typename Iterator>
void parallel_call(const Function&& f, const Iterator first, const Iterator last)
{
    unsigned long const length = std::distance(first, last);
    if(!length)
        return;
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads = 2;
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    std::vector<std::thread> threads(num_threads - 1);
    Iterator block_start = first;
    for(unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(f, block_start, block_end);
        block_start = block_end;
    }
    f(block_start, last);
    for(auto& entry : threads)
        entry.join();
}

template<typename Iterator>
void func1(Iterator first, Iterator last)
{
    for(Iterator it = first; it != last; ++it){
        if(*it)
            st1.push(0);
        else
            st1.pop();
    }
}

void func2()
{
    for(int i = 0; i < MAX_N; ++i){
        if(ok[i])
            st2.push(0);
        else
            st2.pop();
    }
}

// mutex 管理するとシーケンシャルの場合と比べて 20 倍くらい重くなる(8 コア 実質 6.5 コアぐらい)
// (もはやパラレルでもないので mutex のベンチマークという感じ)
// 2 コアでやると 14 倍とか
int main()
{
    ok.resize(MAX_N);
    for(int i = 0; i < MAX_N; ++i){
        st1.push(i), st2.push(i);
        ok[i] = i % 2;
    }
    SW sw1;
    sw1.start();
    parallel_call(func1<vector<int>::iterator>, ok.begin(), ok.end());
    sw1.end();
    SW sw2;
    sw2.start();
    func2();
    sw2.end();
    return 0;
}
