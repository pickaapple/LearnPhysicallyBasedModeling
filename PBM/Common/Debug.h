#pragma once
#include <string>
using namespace std;
class Debug
{
public:
	static void Log(const string& info);
	static void LogWarning(const string& info);
	static void LogError(const string& info);
};

