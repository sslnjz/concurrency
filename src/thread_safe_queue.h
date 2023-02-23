//
// Created by ssln on 2019-05-28.
//

#ifndef CONCURRENCY_THREAD_SAFE_QUEUE_H
#define CONCURRENCY_THREAD_SAFE_QUEUE_H

#include <mutex>
#include <queue>
#include <condition_variable>

namespace concurrent
{
    template <typename T>
    class thread_safe_queue
    {
    private:
        mutable std::mutex _mtx;
        std::condition_variable _wait_condition;
        std::queue<T> _data;

    public:
        thread_safe_queue() : _data(std::queue<T> ()) {}

        thread_safe_queue(const thread_safe_queue& other)
        {
            std::lock_guard locker(other._mtx);
            _data = other;
        }

        thread_safe_queue &operator= (const thread_safe_queue& other) = delete;

        bool empty() const
        {
            std::lock_guard locker(_mtx);
            return _data.empty();
        }

        void push(const T& value)
        {
            std::lock_guard locker(_mtx);
            _data.push(value);
            _wait_condition.notify_one();
        }

        //wait if queue is empty
        std::shared_ptr<T> wait_pop()
        {
            std::unique_lock locker(_mtx);
            _wait_condition.wait(locker, [this]{ return !_data.empty(); });

            std::shared_ptr<T> s_ptr(std::make_shared<T>(_data.front()));
            _data.pop();

            return s_ptr;
        }

        //wait if queue is empty
        void wait_Pop(T& value)
        {
            std::unique_lock locker(_mtx);
            _wait_condition.wait(locker, [this]{ return _data.empty();});

            value = _data.front();
            _data.pop();
        }

        bool try_pop(T& value)
        {
            std::lock_guard locker(_mtx);

            if(_data.empty())
                return false;

            value = _data.front();
            _data.pop();

            return true;
        }

        std::shared_ptr<T> try_pop()
        {
            std::lock_guard locker(_mtx);
            if(_data.empty())
                return std::shared_ptr<T>();

            std::shared_ptr<T> s_ptr(std::make_shared<T>(_data.front()));
            _data.pop();
            return s_ptr;
        }

    };
}
#endif //CONCURRENCY_THREAD_SAFE_QUEUE_H
