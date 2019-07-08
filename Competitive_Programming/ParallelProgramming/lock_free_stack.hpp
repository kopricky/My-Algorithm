#include "parallel_header.hpp"

// pop 時の exception を handle するために T data を持って shared_ptr<T> を pop 時に返すこともできるが, shared_ptr<T> の
// 参照先であるデータ(node) を push 時に new などで heap 領域に確保する必要があるが、 結局領域を確保する際に
// exception を吐く可能性があるのであまり良くない

template<typename T>
class LockFreeStack
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        node* next;
        node(const T& data_) : data(std::make_shared<T>(data_)){}
    };
    std::atomic<node*> head;
public:
    void push(const T& data)
    {
        node* const new_node = new node(data);
        new_node->next = head.load();
        while(!head.compare_exchange_weak(new_node->next, new_node));
    }
    std::shared_ptr<T> pop()
    {
        node* old_head = head.load();
        while(old_head && !head.compare_exchange_weak(old_head, old_head->next));
        return old_head ? old_head->data : std::shared_ptr<T>();
    }
};
