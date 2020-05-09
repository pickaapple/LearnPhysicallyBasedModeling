#include "Debug.h"
#include <iostream>
void Debug::Log(const string & info)
{
	cout << "Log:  " << info << endl;
}

void Debug::LogWarning(const string & info)
{
	cout << "Warning:  " << info << endl;
}


void Debug::LogError(const string & info)
{
	cout << "Error:  " << info << endl;
}
