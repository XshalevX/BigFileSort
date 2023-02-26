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

	void sortChunks(std::vector<std::string> chunks);
	void writeNewChunk(const std::unique_ptr<char>& readBuffer, const int& chunkNumber);
public:
	FileSort(int maxFileSizeBytes, int numberOfLinesPerSegment, int lineSizeBytes);
	
	void Sort(const std::string& inFilePath, const std::string& outFilePath);



};