#include "thread_sequence.h"
#include "thread_pool.h"

int main()
{
   concurrency::thread_sequence ts;
   ts.start(3, 13);

   concurrency::thread_pool pool(3);
   pool.emplace_task([](int i){ std::cout << i << std::endl; }, 42);

   return 0;
}