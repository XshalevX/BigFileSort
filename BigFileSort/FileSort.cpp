#include "FileSort.h"



FileSort::FileSort(int maxFileSizeBytes, int numberOfLinesPerSegment, int lineSizeBytes):
	_maxFileSizeBytes(maxFileSizeBytes), _numberOfLinesPerSegment(numberOfLinesPerSegment), _lineSizeBytes(lineSizeBytes) {}

void FileSort::Sort(const std::string& inFilePath, const std::string& outFilePath)
{
    int byteReadCounter = 0;
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
        CloseHandle(hInfile);
        return;
    }
    DWORD numberOfBytesRead = 0;
    do
    {
        std::unique_ptr<char> readBuffer(new char[_numberOfLinesPerSegment * _lineSizeBytes + 1]);
        
        if (!ReadFile(hInfile, static_cast<char*>(readBuffer.get()), _numberOfLinesPerSegment * _lineSizeBytes, &numberOfBytesRead, NULL))
        {
            std::cerr << "Error in ReadFile" << std::endl;
            CloseHandle(hInfile);

            return;
        }
        if (numberOfBytesRead == 0)
        {
            break;
        }
        if (numberOfBytesRead != _numberOfLinesPerSegment * _lineSizeBytes)
        {
            std::cerr << "File not in currect size!" << std::endl;
            CloseHandle(hInfile);
            return;
        }
        if (readBuffer.get()[_numberOfLinesPerSegment * _lineSizeBytes - 1] != '\n')
        {
            std::cerr << "Too many bytes per line\n";
            CloseHandle(hInfile);
            return;
        }
        

        byteReadCounter += numberOfBytesRead;
        readBuffer.get()[_numberOfLinesPerSegment * _lineSizeBytes] = '\0';
        writeNewChunk(readBuffer, chunkCounter);

        chunkCounter++;
        
        std::cout << readBuffer << std::endl;
        
    }while(hInfile && numberOfBytesRead != 0);

    //delete it
    CloseHandle(hInfile);
    
    sortChunks(HelperUtils::createChunksName(chunkCounter));
    auto queue = getFirstPQueue(HelperUtils::createChunksName(chunkCounter));
    queue.top();
    

}


void FileSort::sortChunks(std::vector<std::string> chunks)
{
    for (auto chunkFilePath : chunks)
    {
        auto chunkReadHandle = HelperUtils::openFile(chunkFilePath, 'r');
        std::unique_ptr<char> readBuffer(new char[_numberOfLinesPerSegment * _lineSizeBytes]);
        if (!ReadFile(chunkReadHandle, static_cast<char*>(readBuffer.get()), _numberOfLinesPerSegment * _lineSizeBytes, NULL, NULL))
        {
            throw "Bla";
        }
        auto lines = split(readBuffer.get(), _numberOfLinesPerSegment * _lineSizeBytes, '\n');
        std::sort(lines.begin(), lines.end());
        CloseHandle(chunkReadHandle);
        auto chunkWriteHandle = HelperUtils::openFile(chunkFilePath, 'w');
        std::string combainedString = "";
        for (auto line : lines)
        {
            combainedString += line;
        }
        WriteFile(chunkWriteHandle, combainedString.c_str(), _numberOfLinesPerSegment * _lineSizeBytes, NULL, NULL);
        CloseHandle(chunkWriteHandle);
    }

}

void FileSort::writeNewChunk(const std::unique_ptr<char>& readBuffer, const int& chunkNumber)
{
    HANDLE newChunkFile;
    try
    {
        newChunkFile = HelperUtils::openFile(std::format("chunk{}.txt", chunkNumber), 'w');
        WriteFile(newChunkFile, readBuffer.get(), _numberOfLinesPerSegment * _lineSizeBytes, NULL, NULL);

    }
    catch (std::exception& err)
    {
        std::cerr << err.what();
        return;
    }
    CloseHandle(newChunkFile);
}

priority_queue FileSort::getFirstPQueue(std::vector<std::string> chunksName)
{
    std::vector<std::string> preSortedValues;
    for (auto chunkFilePath : chunksName)
    {
        auto chunkReadHandle = HelperUtils::openFile(chunkFilePath, 'r');
        std::unique_ptr<char> readBuffer(new char[_lineSizeBytes + 1 ]);
        if (!ReadFile(chunkReadHandle, static_cast<char*>(readBuffer.get()), _lineSizeBytes, NULL, NULL))
        {
            throw "Bla";
        }
        readBuffer.get()[_lineSizeBytes] = '\0';
        preSortedValues.push_back(std::string(readBuffer.get()));
        CloseHandle(chunkReadHandle);
        removeFirstLine(chunkFilePath);
    }
    priority_queue sortedQueue(std::begin(preSortedValues), std::end(preSortedValues));
    return sortedQueue;
}

void FileSort::removeFirstLine(std::string filePath)
{
    auto chunkAppendHandle = HelperUtils::openFile(filePath, 'r');
    SetFilePointer(chunkAppendHandle, _lineSizeBytes, NULL, FILE_BEGIN);
    auto newFileHanle = HelperUtils::openFile("temp.txt", 'w');
    std::unique_ptr<char> readBuffer(new char[ _lineSizeBytes + 1]);
    DWORD numberOfReadBytes = 0;
    while (ReadFile(chunkAppendHandle, readBuffer.get(), _lineSizeBytes, &numberOfReadBytes, NULL))
    {
        if (numberOfReadBytes == 0)
            break;
        readBuffer.get()[_lineSizeBytes] = '\0';
        WriteFile(newFileHanle, readBuffer.get(), numberOfReadBytes, NULL, NULL);
    }
    CloseHandle(chunkAppendHandle);
    CloseHandle(newFileHanle);
    DeleteFileA(filePath.c_str());
    rename("temp.txt", filePath.c_str());
}
