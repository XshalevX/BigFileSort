#pragma once
#include <exception>

class InvaildInputExeption : public std::exception
{
public:
	const char* what() const noexcept override;
};