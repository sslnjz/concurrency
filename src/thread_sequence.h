//
// Created by  on 2019-11-17.
//

#ifndef CONCURRENCY_THREAD_SEQUENCE_H
#define CONCURRENCY_THREAD_SEQUENCE_H

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <algorithm>
#include <functional>

namespace concurrency {

   /**
   **  Aim to solve the problem of multiple threads printing natural numbers in order.
   **  @Usage:
   **        thread_sequence seq;
   **        seq.start(3, 10);
   **
   **        After runing, the console will print like following:
   **           Thread 1: 1
   **           Thread 2: 2
   **           Thread 3: 3
   **           Thread 1: 4
   **           Thread 2: 5
   **           Thread 3: 6
   **           Thread 1: 7
   **           Thread 2: 8
   **           Thread 3: 9
   **           Thread 1: 10
   **  @parameter:
   **        number_of_thread   : The number of threads which will determine how much threads will join in the task
   **        max_number_of_print: The maximum of printed natural numbers, the range is [1, print_max]
   **/
   class thread_sequence
   {
   public:
      thread_sequence() 
         :  _print_seq(0)
         , _index(1) 
      {
      }

      void start(const int number_of_thread, const int max_number_of_print)
      {
         _thread_num = number_of_thread;
         _print_max = max_number_of_print;

         for (size_t i = 0; i < _thread_num; ++i)
            _vector.emplace_back(std::thread(std::bind(&thread_sequence::sequence_print, this, i + 1)));
         std::for_each(_vector.begin(), _vector.end(), std::mem_fn(&std::thread::join));
      }

   private:
      void sequence_print(int index)
      {
         while (_print_seq <= _print_max)
         {
            std::unique_lock<std::mutex> lock(_mutex);
            _condition.wait(lock, [&]() {return index == _index; });
            _index = (index % _thread_num) + 1;
            
            if (_print_seq < _print_max)
               std::cout << "Thread " << index << ": " << ++_print_seq << std::endl;
            else{ 
               _condition.notify_all();
               break;
            }
            _condition.notify_all();
         }
      }

   private:
      std::mutex _mutex;
      std::condition_variable _condition;

      int _thread_num;
      int _print_seq;
      int _index;
      int _print_max;

      std::vector<std::thread> _vector;
   };
}



#endif // CONCURRENCY_THREAD_SEQUENCE_H
