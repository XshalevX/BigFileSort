#pragma once
#include <iostream>
#include <windows.h>
#include <fileapi.h>
#include <vector>
#include <format>
#include <queue>

#include "HelperUtils.h"

using priority_queue = std::priority_queue<std::string, std::vector<std::string>, std::greater<std::string>>;

class FileSort
{
private:
	unsigned int _maxFileSizeBytes;
	unsigned int _numberOfLinesPerSegment;
	unsigned int _lineSizeBytes;

	void sortChunks(std::vector<std::string> chunks);
	void writeNewChunk(const std::unique_ptr<char>& readBuffer, const int& chunkNumber);
	priority_queue getFirstPQueue(std::vector<std::string> chunksName);
	void removeFirstLine(std::string filePath);

public:
	
	FileSort(int maxFileSizeBytes, int numberOfLinesPerSegment, int lineSizeBytes);
	void Sort(const std::string& inFilePath, const std::string& outFilePath);



};