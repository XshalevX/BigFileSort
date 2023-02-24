#pragma once
#include <iostream>
#include <Windows.h>

#include "InvaildInputExeption.h"
#include "InvaildHandleException.h"

#define READ 'r'
#define WRITE 'w'

class HelperUtils
{
public:
	static std::shared_ptr<wchar_t> getWindowsPstr(const std::string& originalStr);
	static HANDLE openFile(const std::string& filePath, char readOrWrite);
};

