#include "../header.hpp"

//CPUベースの時間
struct SW {
    clock_t st;
    void start() {
        st = clock();
    }
    void end() {
        cout << (clock() - st) * 1000 / CLOCKS_PER_SEC << " msec\n";
    }
};

struct SW {
    chrono::high_resolution_clock::time_point st,ed;
    void start() {
        st = chrono::high_resolution_clock::now();
    }
    void end() {
        ed = chrono::high_resolution_clock::now();
        cout << chrono::duration_cast<chrono::milliseconds>(ed - st).count() << " msec\n";
    }
};
