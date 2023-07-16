#pragma once
#include "Queue.h"
#include "Message.h"
#include <shared_mutex>
#include <unordered_map>

namespace TSML
{

    template <typename ThreadMessageEnum>
    class TSMLContext
    {
    public:
        TSMLContext() = default;
        ~TSMLContext() = default;

        void emplace_ts_queue(const std::string& thread_id)
        {
            std::unique_lock<std::shared_mutex> lock(mut);
            queue_map.insert(std::make_pair(thread_id, Queue<Message<ThreadMessageEnum>>()));
            size++;
        }

        void erase_ts_queue(const std::string& thread_id)
        {
            std::unique_lock<std::shared_mutex> lock(mut);
            queue_map.erase(thread_id);
            size--;
        }

        Queue<Message<ThreadMessageEnum>>& get_ts_queue(const std::string& thread_id)
        {
            std::shared_lock<std::shared_mutex> s_lock(mut);
            return queue_map.at(thread_id);
        }

        size_t get_size() const
        {
            std::shared_lock<std::shared_mutex> s_lock(mut);
            return size;
        }

    private:
        std::unordered_map<std::string, Queue<Message<ThreadMessageEnum>> > queue_map;
        size_t size = 0;
        mutable std::shared_mutex mut;
    };

}
