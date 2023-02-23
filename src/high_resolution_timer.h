//
// Created by ssln on 2019-12-4.
//
#ifndef HIGH_RESOLUTION_TIMER_H
#define HIGH_RESOLUTION_TIMER_H

#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

namespace concurrent {
    class high_resolution_timer {
    public:
        template<class F>
        void setTimeout(F fun, int milliseconds) {
            std::thread t([&]() {
                if (_clear.load(std::memory_order_seq_cst)){
                    return;
                }

                high_resolution_clock::time_point tp_start = high_resolution_clock::now();
                while (true) {
                    uint64_t duration = duration_cast<std::chrono::milliseconds>
                            (high_resolution_clock::now() - tp_start).count();
                    if (duration >= milliseconds) {
                        fun();
                        break;
                    }
                }
            });
            t.detach();
        }

        template<typename F>
        void setInterval(F fun, int milliseconds) {
            std::thread t([&]() {
                high_resolution_clock::time_point tp_start = high_resolution_clock::now();
                while (!_clear.load(std::memory_order_seq_cst)) {
                    uint64_t duration = duration_cast<std::chrono::milliseconds>
                            (high_resolution_clock::now() - tp_start).count();
                    if (duration >= milliseconds) {
                        fun();
                        tp_start = high_resolution_clock::now();
                    }
                }
            });
            t.detach();
        }

        void sleep_for(int milliseconds)
        {
            std::thread t([&]() {
                high_resolution_clock::time_point tp_start = high_resolution_clock::now();
                while (!_clear.load(std::memory_order_seq_cst)) {
                    uint64_t duration = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - tp_start).count();
                    if (duration >= milliseconds) {
                        return;
                    }
                }
            });
            t.join();
        }

        void stop() {
            _clear.store(true, std::memory_order_seq_cst);
        }

    private:
        std::atomic<bool> _clear = ATOMIC_VAR_INIT(false);
    };
}


#endif // !HIGH_RESOLUTION_TIMER_H


