#ifndef __THREADMODULE_H__
#define __THREADMODULE_H__
#include <thread>
#include <list>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "util.h"

namespace olib {
	template <typename T>
	class IRunner {
	public:
		virtual ~IRunner() {}

		virtual bool Execute(T * t) = 0;
		virtual void Release() = 0;
	};

	template <typename T>
	class IRunnerFactory {
	public:
		virtual ~IRunnerFactory() {}

		virtual IRunner<T> * Create() = 0;
	};

	template <typename T>
	class ThreadModule {
		template <bool valueSet>
		class BoolGuard {
		public:
			explicit BoolGuard(bool& value) : _value(value) {}
			~BoolGuard() { _value = valueSet; }

		private:
			bool& _value;
		};

		class Thread {
		public:
			Thread() : _module(nullptr), _runner(nullptr), _terminate(false), _running(false) {}
			~Thread() {}

			bool Start(ThreadModule * module, IRunner<T> * runner) {
				_runner = runner;
				_module = module;
				_running = true;
				_thread = std::thread(&Thread::ThreadProc, this);
				return true;
			}

			void Terminate() {
				_terminate = true;
				while (_running)
					_runningCondition.notify_all();
				_thread.join();
				_runner->Release();
			}

			void Push(T * command) {
				std::unique_lock<std::mutex> guard(_runningMutex);
				_runningCommands.push_back(command);
				if (_runningCommands.size() == 1)
					_runningCondition.notify_one();
			}

			void ThreadProc(){
				_running = true;
				BoolGuard<false> boolGuard(_running);
				while (!_terminate) {
					T * command = nullptr;
					while (!_terminate) {
						std::unique_lock<std::mutex> guard(_runningMutex);
						if (_runningCommands.empty())
							_runningCondition.wait(guard);
						if (!_runningCommands.empty()) {
							command = *_runningCommands.begin();
							_runningCommands.pop_front();
						}
					}
					if (command) {
						_runner->Execute(command);
						_module->AddToComplete(command);
					}
				}
				_running = false;
			}

		private:
			ThreadModule * _module;
			IRunner<T> * _runner;
			std::thread _thread;
			bool _terminate;
			bool _running;
			std::mutex _runningMutex;
			std::condition_variable _runningCondition;
			std::list<T*> _runningCommands;
		};

	public:
		ThreadModule() {}
		~ThreadModule() {}

		bool Start(s32 threadCount, IRunnerFactory<T> * factory) {
			for (s32 i = 0; i < threadCount; ++i) {
				auto thread = NEW Thread;
				auto runner = factory->Create();
				if (runner == nullptr) {
					OASSERT(false, "create runner failed");
					DEL thread;
					return false;
				}

				if (!thread->Start(this, runner)) {
					OASSERT(false, "start thread failed");
					DEL thread;
					return false;
				}
				_threads.push_back(thread);
			}
			return true;
		}

		void Terminate() {
			for (auto * thread : _threads) {
				thread->Terminate();
				DEL thread;
			}
			_threads.clear();
		}

		void Process(s64 duration) {
			bool swaped = false;
			while (true) {
				if (!swaped && _completeCommandsRun.empty()) {
					swaped = true;
					std::unique_lock<std::mutex> guard(_completeMutex);
					_completeCommandsRun.swap(_completeCommands);
				}

				if (_completeCommandsRun.empty())
					break;

				T * command = *_completeCommandsRun.begin();
				_completeCommandsRun.pop_front();

				Complete(command);
			}
		}

		void Push(s64 id, T * command) {
			auto * thread = _threads[id % _threads.size()];
			thread->Push(command);
		}

		void AddToComplete(T * command) {
			std::unique_lock<std::mutex> guard(_completeMutex);
			_completeCommands.push_back(command);
		}

		virtual void Complete(T * command) = 0;

	private:
		std::vector<Thread*> _threads;
		std::mutex _completeMutex;
		std::list<T*> _completeCommands;
		std::list<T*> _completeCommandsRun;
	};
}

#endif //__THREADMODULE_H__
