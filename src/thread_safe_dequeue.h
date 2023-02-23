/*****************************************************************************
Copyright:   © 2020 ssln. All rights reserved.
File name:   thread_safe_dequeue.h
Description: 
Author:      ssln
Version:     1.0
Date:        12/19/20
History: 
    1. ssln  12/19/20  1.0  Create
*****************************************************************************/

#ifndef CONCURRENCY_THREAD_SAFE_DEQUEUE_H
#define CONCURRENCY_THREAD_SAFE_DEQUEUE_H

#include <utility>
#include <atomic>
#include <queue>
#include <optional>
#include <cassert>
#include <condition_variable>

namespace concurrent
{
    template <typename T>
    class thread_safe_dequeue
    {
    public:
        /* Create Tsqueue object. Set maximum size of the queue to max_size. */
        inline thread_safe_dequeue(size_t max_size = -1UL) : maxsize(max_size), end(false) {};

        /* Push T to the queue. Many threads can push at the same time.
         * If the queue is full, calling thread will be suspended until
         * some other thread pop() data. */
        void push(const T&);
        void push(T&&);

        /* Close the queue.
         * Be sure all writing threads done their writes before call this.
         * Push data to closed queue is forbidden. */
        void close();

        /* Pop and return T from the queue. Many threads can pop at the same time.
         * If the queue is empty, calling thread will be suspended.
         * If the queue is empty and closed, nullopt returned. */
        std::optional<T> pop();
    private:
        std::queue<T> que;
        std::mutex mtx;
        std::condition_variable cv_empty, cv_full;
        const size_t maxsize;
        std::atomic<bool> end;

    };

    template<typename T>
    void thread_safe_dequeue<T>::push(T&& t)
    {
        std::unique_lock<std::mutex> locker(mtx);
        while (que.size() == maxsize && !end)
            cv_full.wait(locker);
        assert(!end);
        que.push(std::move(t));
        cv_empty.notify_one();
    }

    template<typename T>
    void thread_safe_dequeue<T>::push(const T& t)
    {
        std::unique_lock<std::mutex> locker(mtx);
        while (que.size() == maxsize && !end)
            cv_full.wait(locker);
        assert(!end);
        que.push(std::move(t));
        cv_empty.notify_one();
    }

    template<typename T>
    std::optional<T> thread_safe_dequeue<T>::pop()
    {
        std::unique_lock<std::mutex> locker(mtx);
        while (que.empty() && !end)
            cv_empty.wait(locker);
        if (que.empty()) return {};
        T t = std::move(que.front());
        que.pop();
        cv_full.notify_one();
        return t;
    }

    template<typename T>
    void thread_safe_dequeue<T>::close()
    {
        end = true;
        std::lock_guard<std::mutex> locker(mtx);
        cv_empty.notify_one();
        cv_full.notify_one();
    }
}



#endif //CONCURRENCY_THREAD_SAFE_DEQUEUE_H
