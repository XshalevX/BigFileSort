#pragma once
#include <iostream>
#include <format>
class ReadException : public std::exception
{
private:
	std::string _fileName;
public:
	ReadException(std::string fileName);
	const char* what() const noexcept override;
};