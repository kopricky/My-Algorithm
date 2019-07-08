#include "parallel_header.hpp"

// -std=c++17 をつける
template<typename T>
class ThreadsafeList
{
private:
    struct noexist_erase : std::exception
    {
        const char* what() const noexcept
        {
            return "no element to erase!!";
        };
    };
    struct node
    {
        std::mutex m;
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
        node() : next(){}
        node(const T& value) : data(std::make_shared<T>(value)){}
    };
    node head;
public:
    ThreadsafeList(){}
    ~ThreadsafeList()
    {
        remove_if([](const node&)
        {
            return true;
        });
    }
    ThreadsafeList(const ThreadsafeList& another) = delete;
    ThreadsafeList& operator= (const ThreadsafeList& another) = delete;
    void push_front(const T& value)
    {
        std::unique_ptr<node> new_node(new node(value));
        std::lock_guard<std::mutex> lk(head.m);
        new_node->next = std::move(head.next);
        head.next = std::move(new_node);
    }
    void pop_front()
    {
        std::lock_guard<std::mutex> lk(head.m);
        if(node* const next = head.next.get())
        {
            std::lock_guard<std::mutex> next_lk(next->m);
            std::unique_ptr<node> old_next = std::move(head.next);
            head.next = std::move(next->next);
        }
        else
        {
            noexist_erase err;
            std::cout << err.what() << std::endl;
            assert(false);
        }
    }
    template<typename Function>
    void for_each(const Function f)
    {
        node* current = &head;
        std::unique_lock<std::mutex> lk(head.m);
        while(node* const next = current->next.get())
        {
            std::unique_lock<std::mutex> next_lk(next->m);
            lk.unlock();
            f(*next->data);
            current = next;
            lk = std::move(next_lk);
        }
    }
    template<typename Predicate>
    std::shared_ptr<T> find_first_if(const Predicate p)
    {
        node* current = &head;
        std::unique_lock<std::mutex> lk(head.m);
        while(node* const next = current->next.get())
        {
            std::unique_lock<std::mutex> next_lk(next->m);
            lk.unlock();
            if(p(*next->data))
            {
                return next->data;
            }
            current = next;
            lk = std::move(next_lk);
        }
        return std::shared_ptr<T>();
    }
    template<typename Predicate>
    void remove_if(const Predicate p)
    {
        node* current = &head;
        std::unique_lock<std::mutex> lk(head.m);
        while(node* const next = current->next.get())
        {
            std::unique_lock<std::mutex> next_lk(next->m);
            if(p(*next->data))
            {
                std::unique_ptr<node> old_next = std::move(current->next);
                current->next = std::move(next->next);
                next_lk.unlock();
            }
            else
            {
                lk.unlock();
                current = next;
                lk = std::move(next_lk);
            }
        }
    }
};
