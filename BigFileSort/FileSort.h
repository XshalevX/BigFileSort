#pragma once
#include <iostream>
#include <windows.h>
#include <fileapi.h>
#include <vector>
#include <format>

#include "HelperUtils.h"


class FileSort
{
private:
	unsigned int _maxFileSizeBytes;
	unsigned int _numberOfLinesPerSegment;
	unsigned int _lineSizeBytes;


public:
	FileSort(int maxFileSizeBytes, int numberOfLinesPerSegment, int lineSizeBytes);
	
	void Sort(const std::string& inFilePath, const std::string& outFilePath);



};