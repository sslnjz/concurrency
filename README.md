## C++1x Multiple thread usage 
***thread_pool***  
>A class which will create a pool of thread, you can determine how much threads to create or determined by the hardware performance itself. The ***thread_pool*** class used new **c++1x** features and make it easy to use.   
```c++
    thread_pool pool(4);
    auto result = pool.thread_pool([](int process) { return process; }, 42);
    std::cout << result.get() << std::endl;
```
 ***thread_safe_queue***
>A thread safe queue which used condition variables to try or wait to pop element of the queue.

 ***thread_safe_stack***
>A thread safe stack which used condition variables to try or wait to pop element of the stack.

***thread_sequence***
>Aim to solve the problem of multiple threads printing natural numbers in order.
```c++
    thread_sequence seq;
    seq.start(10, 3);
```
>After running, the console will print like following:
```text
    Thread 1: 1
    Thread 2: 2
    Thread 3: 3
    Thread 1: 4
    Thread 2: 5
    Thread 3: 6
    Thread 1: 7
    Thread 2: 8
    Thread 3: 9
    Thread 1: 10
```
***high_resolution_timer***
>High resolution timer without errors.
```c++
    auto tp = std::chrono::high_resolution_clock::now();
    high_resolution_timer t;
    t.setInterval([&]() {
        printf("Frame:%d\n", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - tp1).count());
        ++frame;
        tp = std::chrono::high_resolution_clock::now();
        }, 10);

    t.sleep_for(1010);
    t.stop();
```
>Console print:
```text
    ---100 ms
    Total: 1010 ms
```
