#include "../threadsafe_unordered_map.hpp"

int main()
{
    int n = 10000000;
    SW sw1, sw2;
    sw1.start();
    std::unordered_map<int, int> mp1;
    sw1.end();
    sw2.start();
    ThreadsafeUnorderedMap<int, int> mp2(n);
    sw2.end();
}
