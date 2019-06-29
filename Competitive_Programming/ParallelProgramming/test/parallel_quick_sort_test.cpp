#include "../parallel_quick_sort.hpp"

using namespace std;

int main()
{
    int n = 10000000;
    vector<int> vec1(n), vec2;
    iota(vec1.begin(), vec1.end(), 0);
    shuffle(vec1.begin(), vec1.end(), mt19937(random_device{}()));
    vec2 = vec1;
    SW sw1, sw2;
    sw1.start();
    parallel_quick_sort(vec1.begin(), vec1.end());
    sw1.end();
    sw2.start();
    sort(vec2.begin(), vec2.end());
    sw2.end();
    assert(vec1 == vec2);
}
