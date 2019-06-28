#include "../parallel_partition.hpp"
#include <cassert>

using namespace std;

int main()
{
    int n = 200000000;
    vector<int> vec1(n), vec2;
    iota(vec1.begin(), vec1.end(), 0);
    shuffle(vec1.begin(), vec1.end(), mt19937(1));
    int pivot = vec1[0];
    vec2 = vec1;
    SW sw1, sw2;
    sw1.start();
    int res1 = parallel_partition(vec1.begin(), vec1.end(), [pivot](const int& itr){ return itr < pivot; }) - vec1.begin();
    sw1.end();
    sw2.start();
    int res2 = std::partition(vec2.begin(), vec2.end(), [pivot](const int& itr){ return itr < pivot; }) - vec2.begin();
    sw2.end();
    assert(res1 == res2);
}
