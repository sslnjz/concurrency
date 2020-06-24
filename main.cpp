#include "thread_sequence.h"
#include "thread_pool.h"
#include "high_resolution_timer.h"

#include <chrono>
#include <future>
#include <thread>
#include <random>
#include <iostream>
#include <exception>
#include <sstream>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::high_resolution_clock;
using concurrent::high_resolution_timer;


// altitude may be altitude::high or altitude::low
enum class altitude : short {
	high = 'h',
	low = 'l', // C++11 allows the extra comma
};



//int doSomething(char c)
//{
//	std::default_random_engine dre(c);
//	std::uniform_int_distribution<int> id(10, 1000);
//
//	for (int i = 0; i < 10; ++i)
//	{
//		std::this_thread::sleep_for(std::chrono::milliseconds(id(dre)));
//		std::cout.put(c).flush();
//	}
//
//	return c;
//}
//
//int func1()
//{
//	return doSomething('.');
//}
//
//int func2()
//{
//	return doSomething('+');
//}

#include <exception>
#include <stdexcept>

int queryNumber()
{
	std::cout << "Read number: ";
	int num{};
	std::cin >> num;

	if (!std::cin)
	{
		throw std::runtime_error("No number read");
	}

	return num;
}


void doSomething(char c, std::shared_future<int> f)
{
	try
	{
		int num = f.get();

		for (int i = 0; i < 10; ++i)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			std::cout.put(c).flush();
		}
	}
	catch (const std::exception e)
	{
		//std::cerr << "EXCEPTION in thread " << std::this_thread::get_id() << ": " << e.what() << std::endl;
		printf("EXCEPTION in thread %d: %s\n", std::this_thread::get_id(), e.what());
	}
}

class B;
class A
{
	friend class B;
public:
	A() {}
	A(int num) : a(num) {}

protected:
	int a;
};

class B
{
private:
	A* a;

	A b;

	A& f;

public:
	B(int num) : f(b)
	{
		a = new A(num);
	}
	void test()
	{
		std::cout << "Access: " << a->a << std::endl;
		a->a = 1000;
		std::cout << "Change: " << a->a << std::endl;
	}
};

#include <iostream>
#include <stdexcept>

class conststr
{
	const char* p;
	std::size_t sz;
public:
	template<std::size_t N>
	constexpr conststr(const char(&a)[N]) : p(a), sz(N - 1) {
		std::cout << "N = " << N << std::endl;
	}

	constexpr char operator[](std::size_t n) const
	{
		return n < sz ? p[n] : throw std::out_of_range("");
	}
	constexpr std::size_t size() const { return sz; }
};

constexpr std::size_t countlower(conststr s, std::size_t n = 0,
	std::size_t c = 0)
{
	return n == s.size() ? c :
		s[n] >= 'a' && s[n] <= 'z' ? countlower(s, n + 1, c + 1) :
		countlower(s, n + 1, c);
}

// 要求编译时常量的输出函数，为测试用
template<int n>
struct constN
{
	constN() { std::cout << n << '\n'; }
};


void print_fun()
{
	//printf("Hello: %d \n", std::this_thread::get_id());

	//printf("This: %d \n", std::this_thread::get_id());

	//printf("IS: %d \n", std::this_thread::get_id());

	std::ostringstream stringbuf;

	stringbuf << "Hello: " << std::this_thread::get_id() << std::endl;

	std::cout << stringbuf.str();

	stringbuf.clear();

	stringbuf << "this: " << std::this_thread::get_id() << std::endl;

	std::cout << stringbuf.str();

	stringbuf.clear();

	stringbuf << "is: " << std::this_thread::get_id() << std::endl;

	std::cout << stringbuf.str();

	stringbuf.clear();
}

inline bool getExecuteThisFrame(
   const int& frame)
{
   if (frame == 0)
   {
      return true;
   }

   //
   // When the frame values for calling are not equal, the current
   // frame is an execution frame.  Integer truncations are
   // intentional and this does not work correctly in frame 0.
   //
   // Osal::Int32 lastFrame = (frame - 1) *
   //                         childFrequency / parentFrequency;
   // Osal::Int32 thisFrame = frame *
   //                         childFrequency / parentFrequency;
   //
   return (((frame - 1) * 50 / 100) != (frame * 50 / 100));
}



int main(int argc, char **argv)
{
   std::vector<int> vec1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
   std::vector<int> vec2;

   auto it = vec1.begin() ;
   auto it2 = std::next(vec1.begin(), vec1.size() / 2);
   std::move(it, it2, std::back_inserter(vec2));
   vec1.erase(it , it2);




 //  std::string x = "10";

 //  size_t index = 0;

 //  const int value = std::stoi(x, &index, 8);

 //  std::cout << "Index = " << index << std::endl;
 //  std::cout << "Value = " << value << std::endl;

	////std::thread t1(print_fun);
	////std::thread t2(print_fun);
	////std::thread t3(print_fun);
	////std::thread t4(print_fun);


	////t1.join();
	////t2.join();
	////t3.join();
	////t4.join();



 //  std::thread t([]()
 //  {

 //     std::thread t2([]()
 //     {
 //        int index = 0;
 //        while (true)
 //        {
 //           ++index;
 //           std::this_thread::sleep_for(std::chrono::seconds(1));

 //           std::cout << index << std::endl;

 //           if(index > 20)
 //              break;
 //        }

 //        std::cout << "child thread exit" << std::endl;
 //     });

 //     t2.detach();

 //     while (true)
 //     {
 //        std::cout << "Main thread sleep" << std::endl;
 //        std::this_thread::sleep_for(std::chrono::seconds(3));
 //     }
 //  });

 //  t.join();


	//B b(50);

	//b.test();

	//std::cout << "the number of lowercase letters in \"Hello, world!\" is ";
	//conststr a("Hello, world!");
	//countlower("Hello, world!");
	//constN<countlower("Hello, world!")>(); // 隐式转换成 conststr

	//try
	//{
	//	std::shared_future<int> f = std::async(queryNumber);

	//	auto f1 = std::async(std::launch::async, doSomething, '.', f);
	//	auto f2 = std::async(std::launch::async, doSomething, '+', f);
	//	auto f3 = std::async(std::launch::async, doSomething, '*', f);

	//	f1.get();
	//	f2.get();
	//	f3.get();
	//}
	//catch (const std::exception e)
	//{
	//	std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	//}


	//std::future<int> result1(std::async(std::launch::async, func1));

	//int result2 = func2();

	//int result = result1.get() + result2;

	//std::cout << "\nresult:" << result << std::endl;

	//std::cout << static_cast<char>(altitude::high) << std::endl;
    //auto tp = high_resolution_clock::now();
    //auto tp1 = high_resolution_clock::now();

    //auto duration = [&](decltype(tp) t){
    //    return duration_cast<milliseconds>(high_resolution_clock::now() - t).count();
    //};

    ////#1 interval
    //high_resolution_timer t;
    //t.setInterval([&]() {
    //    printf("%lld ms\n", duration(tp1));
    //    tp1 = high_resolution_clock::now();
    //    }, 1000 / 100);

    ////#2 timeout
    //t.setTimeout([&] {
    //    printf("---%lld ms\n", duration(tp));
    //    }, 100);

    ////#3 sleep
    //t.sleep_for(1001);

    //t.stop();
    //printf("Total: %lld ms\n", duration(tp));

    //concurrent::thread_sequence ts;
    //ts.start(3, 60);

    return EXIT_SUCCESS;
}