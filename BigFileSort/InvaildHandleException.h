#pragma once
#include <exception>

class InvaildHandleException : public std::exception
{
public:
	const char* what() const noexcept override;
};