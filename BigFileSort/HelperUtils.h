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

std::shared_ptr<wchar_t> getWindowsPstr(const std::string& originalStr);
HANDLE openFile(const std::string& filePath, char readOrWrite, int openOption = 0);
std::vector<std::string> split(const char* str, const int size, const char del);
