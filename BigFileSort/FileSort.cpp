#include "FileSort.h"

FileSort::FileSort(int maxFileSizeBytes, int numberOfLinesPerSegment, int lineSizeBytes):
	_maxFileSizeBytes(maxFileSizeBytes), _numberOfLinesPerSegment(numberOfLinesPerSegment), _lineSizeBytes(lineSizeBytes) {}

void FileSort::Sort(const std::string& inFilePath, const std::string& outFilePath)
{
    int byteReadCounter = 0;
    std::vector<HANDLE> chunks;
    int chunkCounter = 1;

    HANDLE hInfile;
    try
    {
        hInfile = HelperUtils::openFile(inFilePath,'r');
    }
    catch (std::exception& err)
    {
        std::cerr << err.what();
        return;
    }
    if (GetFileSize(hInfile, NULL) > _maxFileSizeBytes)
    {
        std::cerr << "File to big" << std::endl;
        return;
    }
    DWORD numberOfBytesRead = 0;
    do
    {
        std::unique_ptr<char> readBuffer(new char[_numberOfLinesPerSegment * _lineSizeBytes]);
        
        if (!ReadFile(hInfile, static_cast<char*>(readBuffer.get()), _numberOfLinesPerSegment * _lineSizeBytes, &numberOfBytesRead, NULL))
        {
            std::cerr << "Error in ReadFile" << std::endl;
            return;
        }
        if (numberOfBytesRead == 0)
        {
            break;
        }
        if (numberOfBytesRead != _numberOfLinesPerSegment * _lineSizeBytes)
        {
            std::cerr << "File not in currect size!" << std::endl;
            return;
        }
        if (readBuffer.get()[_numberOfLinesPerSegment * _lineSizeBytes - 1] != '\n')
        {
            std::cerr << "Too many bytes per line\n";
            return;
        }
        

        byteReadCounter += numberOfBytesRead;
        readBuffer.get()[_numberOfLinesPerSegment * _lineSizeBytes - 1] = '\0';
        
        HANDLE newChunkFile;
        try
        {
            newChunkFile = HelperUtils::openFile(std::format("chunk{}.txt", chunkCounter), 'w');
        }
        catch (std::exception& err)
        {
            std::cerr << err.what();
            return;
        }

        chunkCounter++;
        chunks.push_back(newChunkFile);
        WriteFile(newChunkFile, readBuffer.get(), _numberOfLinesPerSegment * _lineSizeBytes, NULL, NULL);

        std::cout << readBuffer << std::endl;
        
    }while(hInfile && numberOfBytesRead != 0);
}

