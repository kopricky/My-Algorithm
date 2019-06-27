#include "matplotlib.hpp"
#include <iostream>
#include <thread>
#include <numeric>
#include <vector>
#include <stack>
#include <mutex>
#include <random>
#include <exception>
#include <algorithm>
#include <iterator>
#include <future>

struct SW {
    std::chrono::high_resolution_clock::time_point st,ed;
    void start() {
        st = std::chrono::high_resolution_clock::now();
    }
    int end() {
        ed = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count();
    }
};

unsigned long THRESHOLD;

template<typename Iterator>
void _parallel_quick_sort(Iterator first, Iterator last, std::mt19937& mt)
{
    const unsigned long length = std::distance(first, last);
    if(length <= THRESHOLD){
        if(length >= 2) std::sort(first, last);
        return;
    }
    std::uniform_int_distribution<> uid(0, length);
    auto pivot = *std::next(first, uid(mt));
    Iterator middle1 = std::partition(first, last, [pivot](const auto& itr){ return itr < pivot; });
    Iterator middle2 = std::partition(first, last, [pivot](const auto& itr){ return !(pivot < itr); });
    auto fut1 = std::async(_parallel_quick_sort<Iterator>, first, middle1, std::ref(mt));
    auto fut2 = std::async(_parallel_quick_sort<Iterator>, middle2, last, std::ref(mt));
    fut1.wait(), fut2.wait();
}

template<typename Iterator>
void parallel_quick_sort(Iterator first, Iterator last)
{
    std::mt19937 mt(std::random_device{}());
    _parallel_quick_sort(first, last, mt);
}

using namespace std;
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
    int n = 500000;
    vector<int> x,y;
    vector<int> vec1(n);
    iota(vec1.begin(), vec1.end(), 0);
    shuffle(vec1.begin(), vec1.end(), mt19937(random_device{}()));
    for(int i = 500; i < 500*50; i += 500){
        THRESHOLD = i;
        x.push_back(i);
        vector<int> res = vec1;
        SW sw1;
        sw1.start();
        parallel_quick_sort(res.begin(), res.end());
        y.push_back(sw1.end());
    }
    print(x,y);
}
