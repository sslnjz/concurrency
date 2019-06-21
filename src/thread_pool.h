//
// Created by  on 2019-06-17.
// Related git@github.com:progschj/ThreadPool.git
//

#ifndef CONCURRENCY_THREAD_POOL_H
#define CONCURRENCY_THREAD_POOL_H

#include <vector>
#include <queue>
#include <mutex>
#include <memory>
#include <future>
#include <functional>
#include <condition_variable>

namespace concurrency
{
    class thread_pool
    {
    public:
        thread_pool(size_t size) : _stop(false)
        {
            size_t count = size > std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : size;

            for (int i = 0; i < count; ++i) {
                _workers.emplace_back(
                    [this]{
                        while (true)
                        {
                            std::function<void()> task;
                            {
                                std::unique_lock<std::mutex> locker(_mtx);
                                _wait_conditions.wait(locker, [this]{
                                    return this->_stop || !this->_tasks.empty();
                                });

                                if(_stop && _tasks.empty())
                                    return;

                                task = std::move(_tasks.front());
                                _tasks.pop();
                            }
                            task();
                        }
                    }
                );
            }
        }

        ~thread_pool()
        {
            {
                std::unique_lock<std::mutex> locker(_mtx);
                _stop = true;
            }
            _wait_conditions.notify_all();

            for (auto& work : _workers) {
                if(work.joinable())
                    work.join();
            }
        }

        template <class Func, class... Args>
        auto emplace_task(Func&& f, Args&&... args)
            -> std::future<typename std::result_of<Func(Args...)>::type>
        {
            using _T = typename std::result_of<Func(Args...)>::type;
            auto task = std::make_shared< std::packaged_task<_T()> >(
                    std::bind(std::forward<Func>(f), std::forward<Args>(args)...)
                );

            std::future<_T> res;
            res = task->get_future();
            {
                std::unique_lock<std::mutex> locker(_mtx);
                if(_stop)
                    throw std::runtime_error("stoped pool");

                _tasks.emplace([task](){ (*task)(); });
            }

            _wait_conditions.notify_one();
            return res;
        }

    private:
        std::vector<std::thread> _workers;
        std::queue<std::function<void()>> _tasks;

        std::mutex _mtx;
        std::condition_variable _wait_conditions;
        bool _stop;
    };

}

#endif //CONCURRENCY_THREAD_POOL_H
