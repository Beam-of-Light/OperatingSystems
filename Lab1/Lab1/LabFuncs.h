#ifndef LAB_FUNCS_H
#define LAB_FUNCS_H

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <Windows.h>
#include "demofuncs"

using namespace spos::lab1::demo;

namespace LabFuncs
{
	// Global variables
	int* f_result = nullptr;
	int* g_result = nullptr;
	std::mutex mtx;
	std::condition_variable cv;
	bool thread_notified = false;
	bool canceled = false;

	enum Functions {F, G, F_TEST, G_TEST};

	int f(int func_val)
	{
		switch (func_val) {
		case 1:
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			return 5;
		case 2:
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
			return 3;
		case 3:
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			return 0;
		case 5:
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			return 10;
		case 4:
		case 6:
			std::this_thread::sleep_for(std::chrono::hours(1));
		default:
			return 100;
		}
	}

	int g(int func_val)
	{
		switch (func_val) {
		case 1:
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
			return 7;
		case 2:
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			return 4;
		case 4:
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			return 0;
		case 6:
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			return 15;
		case 3:
		case 5:
			std::this_thread::sleep_for(std::chrono::hours(1));
		default:
			return 100;
		}
	}

	void fgStartThread(Functions func, int func_val)
	{
		if (func == Functions::F) {
			std::thread t([func_val]()
				{
					f_result = new int(f(func_val));
					thread_notified = true;
					cv.notify_one();
				});
			t.detach();
		}
		else if (func == Functions::G) {
			std::thread t([func_val]()
				{
					g_result = new int(g(func_val));
					thread_notified = true;
					cv.notify_one();
				});
			t.detach();
		} else if (func == Functions::F_TEST) {
			std::thread t([func_val]()
				{
					f_result = new int(f_func<spos::lab1::demo::INT>(func_val - 1));
					thread_notified = true;
					cv.notify_one();
				});
			t.detach();
		}
		else if (func == Functions::G_TEST) {
			std::thread t([func_val]()
				{
					g_result = new int(g_func<spos::lab1::demo::INT>(func_val - 1));
					thread_notified = true;
					cv.notify_one();
				});
			t.detach();
		}
	}

	void cancelation()
	{
		while (true) {
			if (GetKeyState('Q') & 0x8000) {
				if (f_result) {
					delete f_result;
				}
				else {
					std::cout << "\nf - undefined";
				}
				if (g_result) {
					delete g_result;
				}
				else {
					std::cout << "\ng - undefined";
				}
				f_result = g_result = nullptr;
				std::cout << "\nResult is undefined\n";
				cv.notify_one();
				canceled = true;
				return;
			}
		}
	}

	void fgHandler(int func_val, int mode)
	{
		// Start f and g in different threads
		if (mode == 1) {
			fgStartThread(Functions::F, func_val);
			fgStartThread(Functions::G, func_val);
		} else if (mode == 2) {
			fgStartThread(Functions::F_TEST, func_val);
			fgStartThread(Functions::G_TEST, func_val);
		} else return;

		// Handle f and g computation
		std::cout << "\nPress Q key to stop computation\n";
		std::thread t(cancelation);
		t.detach();

		int result;
		bool f_check = true, g_check = true;
		std::unique_lock<std::mutex> locker(mtx);
		while (true) {
			while (!thread_notified) {
				cv.wait(locker);
				if (canceled) return;
			}
			if (f_result && f_check) {
				std::cout << "\nf - " << std::to_string(*f_result);
				f_check = false;
			}
			if (g_result && g_check) {
				std::cout << "\ng - " << std::to_string(*g_result);
				g_check = false;
			}
			if (f_result && g_result) {
				result = *f_result * *g_result;
				break;
			}
			if (f_result && *f_result == 0) {
				result = 0;
				std::cout << "\ng - undefined";
				break;
			}
			if (g_result && *g_result == 0) {
				result = 0;
				std::cout << "\nf - undefined";
				break;
			}
			thread_notified = false;
		}
		std::cout << "\nResult is " << std::to_string(result) << std::endl;

		if (f_result) delete f_result;
		if (g_result) delete g_result;
		f_result = g_result = nullptr;
	}

	template<typename T>
	void write_data(T& data, const std::string& msg)
	{
		while (!(std::cin >> data)) {
			std::cin.clear(); /* Clear errors (like the failbit flag) */
			std::cin.ignore(LLONG_MAX, '\n'); /* Throw away the rest of the line */
			std::cout << msg;
		}
	}
};
#endif // !LAB_FUNCS_H