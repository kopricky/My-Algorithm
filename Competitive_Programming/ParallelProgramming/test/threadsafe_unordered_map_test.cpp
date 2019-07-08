#include "../threadsafe_unordered_map.hpp"

int main()
{
    // int n = 10000000;
    ThreadsafeUnorderedMap<int, int> mp2(19);
    mp2[1] = 2;
    mp2.erase(2);
    std::cout << mp2[1] << std::endl;
}
