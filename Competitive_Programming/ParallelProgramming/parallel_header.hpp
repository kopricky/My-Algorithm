#ifndef PARALLEL_HEADER_HPP
#define PARALLEL_HEADER_HPP

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
#include <queue>
#include <functional>
#include <cassert>
#include <memory>
#include <list>
#include <shared_mutex>

struct SW {
    std::chrono::high_resolution_clock::time_point st,ed;
    void start() {
        st = std::chrono::high_resolution_clock::now();
    }
    void end() {
        ed = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count() << " msec\n";
    }
};

#endif
