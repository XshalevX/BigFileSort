#pragma once
#include <iostream>
#include <format>
class WriteException : public std::exception
{
private:
	std::string _fileName;
public:
	WriteException(std::string fileName);
	const char* what() const noexcept override;
};