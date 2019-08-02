#include "parallel_header.hpp"

template<typename T>
class ThreadsafeQueue
{
private:
    class Node
    {
    public:
        std::shared_ptr<T> data;
        std::unique_ptr<Node> next;
    };
    std::mutex head_mutex;
    std::unique_ptr<Node> head;
    std::mutex tail_mutex;
    Node* tail;
    std::condition_variable data_cond;

    Node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }
    std::unique_ptr<Node> pop_head()
    {
        std::unique_ptr<Node> old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }
    std::unique_lock<std::mutex> wait_for_data()
    {
        std::unique_lock<std::mutex> head_lock(head_mutex);
        data_cond.wait(head_lock, [&]
        {
            return head.get() != get_tail();
        });
        return std::move(head_lock);
    }
    std::unique_ptr<Node> try_pop_head()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if(head.get() == get_tail())
        {
            return std::unique_ptr<Node>();
        }
        return pop_head();
    }
    std::unique_ptr<Node> try_pop_head(T& value)
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if(head.get() == get_tail())
        {
            return std::unique_ptr<Node>();
        }
        value = std::move(*head->data);
        return pop_head();
    }
    std::unique_ptr<Node> wait_pop_head()
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        return pop_head();
    }
    std::unique_ptr<Node> wait_pop_head(T& value)
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        value = std::move(*head->data);
        return pop_head();
    }

public:
    ThreadsafeQueue() : head(new Node), tail(head.get()){}
    ThreadsafeQueue(const ThreadsafeQueue& another) = delete;
    ThreadsafeQueue& operator=(const ThreadsafeQueue& another) = delete;
    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<Node> old_head = try_pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }
    bool try_pop(T& value)
    {
        const std::unique_ptr<Node> old_head= try_pop_head(value);
        return old_head;
    }
    std::shared_ptr<T> wait_and_pop()
    {
        const std::unique_ptr<Node> old_head = wait_pop_head();
        return old_head->data;
    }
    void wait_and_pop(T& value)
    {
        const std::unique_ptr<Node> old_head = wait_pop_head(value);
    }
    void push(T new_value)
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<Node> p(new Node);
        {
            std::lock_guard<std::mutex> tail_lock(tail_mutex);
            tail->data = new_data;
            Node* const new_tail = p.get();
            tail->next = std::move(p);
            tail = new_tail;
        }
        data_cond.notify_one();
    }
    bool empty()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        return (head.get() == get_tail());
    }
};
