#include <chrono>
#include <iostream>

#include "Source/Common/Exception.h"

#include "ConsoleControl.h"

int main(int argc, char * argv[])
{
	using namespace solar;
	try
	{
		try
		{
			console::Control(argc, argv);
		}
		catch (const std::exception& e)
		{
			std::cout << "\n\nFollowing exception has been thrown:\n" << e.what() << "\n\n";
			std::cin.get();
			std::cin.get();
		}
	}
	catch (...)
	{
		std::cout << "UNCAUGHT EXCEPTION!";
	}
	return 0;
}


