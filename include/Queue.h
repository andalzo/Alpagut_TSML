#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

namespace TSML
{
    template<typename T>
    class Queue
    {
    public:
        Queue() = default;

        void push(T new_value)
        {
            std::lock_guard<std::mutex> lock(mut);
            data_queue.push(new_value);
            data_cond.notify_one();
        }

        Queue(const Queue<T>& other)
        {
            std::lock_guard<std::mutex> lk(other.mut);
            data_queue=other.data_queue;
        }

        Queue& operator=(const Queue<T>& other)
        {
            if(this != other)
            {
                std::unique_lock<std::mutex> promiseLockThis(mut, std::defer_lock);
                std::unique_lock<std::mutex> promiseLockOther(other.mut, std::defer_lock);
                std::lock(promiseLockThis,promiseLockOther);
                data_queue = other.data_queue;
            }
            return *this;
        }


        //If you need to wait to come specific data at that time, you can use these methods.
        void wait_and_pop(T& value)
        {
            std::unique_lock<std::mutex> lock(mut);
            data_cond.wait(lock, [this] {return !data_queue.empty();});
            value = data_queue.front();
            data_queue.pop();
        }

        std::shared_ptr<T> wait_and_pop()
        {
            std::unique_lock<std::mutex> lock(mut);
            data_cond.wait(lock, [this] {return !data_queue.empty();});
            std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
            data_queue.pop();
            return res;
        }

        //If you do not need to wait to come specific data at that time, you can use these methods.
        bool try_pop(T& value)
        {
            std::lock_guard<std::mutex> lk(mut);
            if(data_queue.empty())
                return false;
            value=data_queue.front();
            data_queue.pop();
            return true;
        }

        std::shared_ptr<T> try_pop()
        {
            std::lock_guard<std::mutex> lk(mut);
            if(data_queue.empty())
                return std::shared_ptr<T>();
            std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
            data_queue.pop();
            return res;
        }
        bool empty() const
        {
            std::lock_guard<std::mutex> lk(mut);
            return data_queue.empty();
        }

    private:
        mutable std::mutex mut;
        std::queue<T> data_queue;
        std::condition_variable_any data_cond;
    };

}


