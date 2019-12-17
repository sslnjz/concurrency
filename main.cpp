#include "thread_sequence.h"
#include "thread_pool.h"
#include "high_resolution_timer.h"

#include <chrono>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::high_resolution_clock;
using concurrent::high_resolution_timer;

int main(int argc, char **argv)
{
    auto tp = high_resolution_clock::now();
    auto tp1 = high_resolution_clock::now();

    auto duration = [&](decltype(tp) t){
        return duration_cast<milliseconds>(high_resolution_clock::now() - t).count();
    };

    //#1 interval
    high_resolution_timer t;
    t.setInterval([&]() {
        printf("%lld ms\n", duration(tp1));
        tp1 = high_resolution_clock::now();
        }, 1000 / 100);

    //#2 timeout
    t.setTimeout([&] {
        printf("---%lld ms\n", duration(tp));
        }, 100);

    //#3 sleep
    t.sleep_for(1001);

    t.stop();
    printf("Total: %lld ms\n", duration(tp));

    concurrent::thread_sequence ts;
    ts.start(3, 60);

    return EXIT_SUCCESS;
}