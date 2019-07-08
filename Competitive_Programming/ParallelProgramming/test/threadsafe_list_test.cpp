#include "../threadsafe_list.hpp"

int main()
{
    ThreadsafeList<int> ls;
    ls.push_front(1);
    ls.pop_front();
}
