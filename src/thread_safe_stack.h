#include <mutex>
#include <exception>
#include <stack>

#ifndef CONCURRENCY_THREAD_SAFE_STACK_H
#define CONCURRENCY_THREAD_SAFE_STACK_H

namespace concurrency {

	struct empty_stack_exception : std::exception {
		const char *what() const noexcept override;
	};

	const char *empty_stack_exception::what() const noexcept {
		return "exception for empty stack";
	}


    template<typename T>
	class thread_safe_stack {
	public:
		thread_safe_stack() : _data(std::stack<T>()) {}

		thread_safe_stack(const thread_safe_stack &other) {
			std::lock_guard locker(other._mtx);
			_data = other._data;
		}

		thread_safe_stack &operator=(const thread_safe_stack &other) = delete;

		bool empty() const {
			std::lock_guard locker(_mtx);
			return _data.empty();
		}

		void push(const T &value) {
			std::lock_guard locker(_mtx);
			_data.push(value);
		}

		void pop(T &top) {
			std::lock_guard locker(_mtx);

			if (_data.empty())
				throw empty_stack_exception();

			top = _data.top();
			_data.pop();
		}

		std::shared_ptr<T> pop() {
			std::lock_guard locker(_mtx);

			if (_data.empty())
				throw empty_stack_exception();

			std::shared_ptr<T> const res( std::make_shared<T>(_data.top()) );
			_data.pop();

			return res;
		}

	private:
		mutable std::mutex _mtx;
		std::stack<T> _data;
	};

}

#endif //CONCURRENCY_THREAD_SAFE_STACK_H