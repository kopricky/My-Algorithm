#include "../parallel_accumulate.hpp"
#include <cassert>

using namespace std;

const int MAX_N = 100000000;

int main()
{
    vector<int> a(MAX_N), b(MAX_N);
    mt19937 mt(random_device{}());
    uniform_int_distribution<> rand100(0, MAX_N);
    for(int i = 0; i < MAX_N; ++i){
        a[i] = b[i] = rand100(mt);
    }
    SW sw1;
    sw1.start();
    long long res1 = parallel_accumulate(a.begin(), a.end(), 0LL);
    sw1.end();
    SW sw2;
    sw2.start();
    long long res2 = accumulate(b.begin(), b.end(), 0LL);
    sw2.end();
    assert(res1 == res2);
    return 0;
}
