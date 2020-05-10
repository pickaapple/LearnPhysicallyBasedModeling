#pragma once
#include <exception>
using namespace std;
class ShaderException : exception
{
public:
	explicit ShaderException(char const* const _Message) noexcept;
};

