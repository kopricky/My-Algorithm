#include "../header.hpp"

//CPUベースの時間
struct SW {
    clock_t st;
    void start() {
        st = clock();
    }
    void end() {
        cout << (clock() - st) * 1000 / CLOCKS_PER_SEC << " msec" << endl;
    }
};


