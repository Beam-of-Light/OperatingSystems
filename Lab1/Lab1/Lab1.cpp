#include "LabFuncs.h"

void demo();

int main()
{
	demo();

    return 0;
}

void demo()
{
	std::cout << "Choose one of the following test-cases:\n"
		<< "\t1 - f finishes before g with non-zero value. Verify the rezult\n"
		<< "\t2 - g finishes before f with non-zero value. Verify the rezult\n"
		<< "\t3 - f finishes with zero value, g hangs. Verify rezult/time\n"
		<< "\t4 - g finishes with zero value, f hangs. Verify rezult/time\n"
		<< "\t5 - f finishes with non-zero value, g hangs. Check cancellation, verify status\n"
		<< "\t6 - g finishes with non-zero value, f hangs. Check cancellation, verify status\n"
		<< "\nWrite number from 1 to 6: ";

	int func_val, mode;
	LabFuncs::write_data<int>(func_val, "\nPlease, enter correct data: ");
	std::cout << "\nChoose mode:\n\t1 - default\n\t2 - test\n";
	LabFuncs::write_data<int>(mode, "\nPlease, enter correct data: ");
	LabFuncs::fgHandler(func_val, mode);
}