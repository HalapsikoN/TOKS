#include "Exception.h"

Exception::Exception()
{
	std::cout << "There is a problem" << std::endl;
}

Exception::Exception(string str)
{
	std::cout << "There is a problem " << str << std::endl;
	
}