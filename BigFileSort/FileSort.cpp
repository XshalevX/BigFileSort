#include "FileSort.h"

fileSort::fileSort(int maxFileSizeBytes, int numberOfLinesPerSegment, int lineSizeBytes):
	_maxFileSizeBytes(maxFileSizeBytes), _numberOfLinesPerSegment(numberOfLinesPerSegment), _lineSizeBytes(lineSizeBytes) {}

