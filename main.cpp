#include "thread_sequence.h"
#include "thread_pool.h"
#include "high_resolution_timer.h"

#include <chrono>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::high_resolution_clock;
using concurrent::high_resolution_timer;

int main(int argc, char** argv)
{
   auto tp = high_resolution_clock::now();

   high_resolution_timer t;
   auto tp1 = high_resolution_clock::now();
   t.setInterval([&]() {
      std::cout << duration_cast<milliseconds>(high_resolution_clock::now() - tp1).count() << " .ms" << std::endl;
      tp1 = std::chrono::high_resolution_clock::now();
      }, 1);

   std::this_thread::sleep_for(std::chrono::milliseconds(100));

   t.stop();

   std::cout << "Total:" <<duration_cast<milliseconds>(high_resolution_clock::now() - tp).count() << " .ms" << std::endl;

   return 0;
}