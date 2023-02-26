#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include <format>

#include "InvaildInputExeption.h"
#include "InvaildHandleException.h"

#define READ 'r'
#define WRITE 'w'
#define APPEND 'a'

class HelperUtils
{
public:
	static std::shared_ptr<wchar_t> getWindowsPstr(const std::string& originalStr);
	static HANDLE openFile(const std::string& filePath, char readOrWrite, int openOption = 0);

	static std::vector<std::string> createChunksName(int numberOfChunks);
	//static std::vector<std::string> split(const char* str, const int size, const char del);

};

std::vector<std::string> split(const char* str, const int size, const char del);
