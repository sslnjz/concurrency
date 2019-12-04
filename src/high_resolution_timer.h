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
using std::chrono::milliseconds;
using std::chrono::high_resolution_clock;

namespace concurrent {

    class high_resolution_timer {
    public:
        template<typename Fun>
        void setTimeout(Fun function, int delay) {
            std::thread t([&, function, delay]() {
                if (_clear.load(std::memory_order_seq_cst))
                    return;
                high_resolution_clock::time_point tp_start = high_resolution_clock::now();
                while (true) {
                    uint64_t duration = duration_cast<milliseconds>(high_resolution_clock::now() - tp_start).count();
                    if (duration >= delay) {
                        function();
                        break;
                    }
                }
            });
            t.detach();
        }

        template<typename Fun>
        void setInterval(Fun function, int interval) {
            std::thread t([&, function, interval]() {
                high_resolution_clock::time_point tp_start = high_resolution_clock::now();
                while (!_clear.load(std::memory_order_seq_cst)) {
                    uint64_t duration = duration_cast<milliseconds>(high_resolution_clock::now() - tp_start).count();
                    if (duration >= interval) {
                        function();
                        tp_start = high_resolution_clock::now();
                    }
                }
            });
            t.detach();
        }

        void stop() {
            _clear.store(true, std::memory_order_seq_cst);
        }

    private:
        std::atomic<bool> _clear = ATOMIC_VAR_INIT(false);
    };
}


#endif // !HIGH_RESOLUTION_TIMER_H


