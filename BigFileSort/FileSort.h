#pragma once
#include <iostream>
#include <windows.h>
#include <fileapi.h>
#include <vector>
#include <format>
#include <queue>
#include <map>

#include "HelperUtils.h"

using wordData = std::pair <std::string, std::string>;

using priority_queue = std::priority_queue<wordData, std::vector<wordData>, std::greater<wordData>>;

class FileSort
{
private:
	unsigned int _maxFileSizeBytes;
	unsigned int _numberOfLinesPerSegment;
	unsigned int _lineSizeBytes;

	void sortChunks(std::vector<std::string>& chunks) const;
	void writeNewChunk(const std::unique_ptr<char>& readBuffer, const int& chunkNumber) const;
	priority_queue getFirstPQueue(std::vector<std::string>& chunks) const;
	bool removeFirstLine(std::string filePath) const;
	wordData getFirstLine(std::string chunk, std::vector<std::string>& chunks) const;

public:
	
	FileSort(int maxFileSizeBytes, int numberOfLinesPerSegment, int lineSizeBytes);
	void Sort(const std::string& inFilePath, const std::string& outFilePath) const;



};