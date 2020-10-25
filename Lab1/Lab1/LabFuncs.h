#ifndef LAB_FUNCS_H
#define LAB_FUNCS_H

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include "demofuncs"

using namespace spos::lab1::demo;

namespace LabFuncs
{
	// Global variables
	int* f_result = nullptr;
	int* g_result = nullptr;

	enum Functions {F, G};

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
				});
			t.detach();
		}
		else {
			std::thread t([func_val]()
				{
					g_result = new int(g(func_val));
				});
			t.detach();
		}
	}

	void fgHandler(int func_val)
	{
		// Start f and g in different threads
		fgStartThread(Functions::F, func_val);
		fgStartThread(Functions::G, func_val);

		// Handle f and g computation
		int result;
		while (true) {
			if (f_result && g_result) {
				result = *f_result * *g_result;
				break;
			}
			if (f_result && *f_result == 0) {
				result = 0;
				break;
			}
			if (g_result && *g_result == 0) {
				result = 0;
				break;
			}
		}
		std::cout << "\nResult is " << result;

		if (f_result) delete f_result;
		if (g_result) delete g_result;
		f_result = g_result = nullptr;
	}

	template<typename T>
	void write_data(T& data, const std::string& msg)
	{
		while (!(std::cin >> data)) {
			std::cin.clear(); /* Clear errors (like the failbit flag) */
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); /* Throw away the rest of the line */
			std::cout << msg;
		}
	}
};
#endif // !LAB_FUNCS_H