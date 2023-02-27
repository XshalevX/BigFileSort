#include "FileSort.h"



FileSort::FileSort(unsigned long maxFileSizeBytes, int numberOfLinesPerSegment, int lineSizeBytes):
	_maxFileSizeBytes(maxFileSizeBytes), _numberOfLinesPerSegment(numberOfLinesPerSegment), _lineSizeBytes(lineSizeBytes) {}

void FileSort::Sort(const std::string& inFilePath, const std::string& outFilePath) const
{
    int byteReadCounter = 0;
    int chunkCounter = 1;

    HANDLE hInfile;
    HANDLE finalSortedFile;
    std::vector<std::string> chunksData;


    hInfile = HelperUtils::openFile(inFilePath,'r');
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
        chunksData.push_back(std::format("chunk{}.txt", chunkCounter));

        writeNewChunk(readBuffer, chunkCounter);
        

        chunkCounter++;
                
    }while(hInfile && numberOfBytesRead != 0);

    //delete it
    CloseHandle(hInfile);
    

    sortChunks(chunksData);
    

    finalSortedFile = HelperUtils::openFile(outFilePath, 'w');
    auto queue = getFirstPQueue(chunksData);

    while (!chunksData.empty())
    {
        auto data = queue.top();
        WriteFile(finalSortedFile, data.first.c_str(), data.first.size(), NULL, NULL);
        auto newLine = getFirstLine(data.second, chunksData);
        queue.pop();
        queue.push(newLine);

    }
    

    

}


void FileSort::sortChunks(std::vector<std::string>& chunks) const
{
    HANDLE chunkWriteHandle;
    HANDLE chunkReadHandle;
    for (auto chunkFilePath : chunks)
    {

        chunkReadHandle = HelperUtils::openFile(chunkFilePath, 'r');
        std::unique_ptr<char> readBuffer(new char[_numberOfLinesPerSegment * _lineSizeBytes]);
        if (!ReadFile(chunkReadHandle, static_cast<char*>(readBuffer.get()), _numberOfLinesPerSegment * _lineSizeBytes, NULL, NULL))
        {
            throw "Bla";
        }
        auto lines = split(readBuffer.get(), _numberOfLinesPerSegment * _lineSizeBytes, '\n');
        std::sort(lines.begin(), lines.end());
        CloseHandle(chunkReadHandle);
        chunkWriteHandle = HelperUtils::openFile(chunkFilePath, 'w');
        std::string combainedString = "";
        for (auto line : lines)
        {
            combainedString += line;
        }
        WriteFile(chunkWriteHandle, combainedString.c_str(), _numberOfLinesPerSegment * _lineSizeBytes, NULL, NULL);
        CloseHandle(chunkWriteHandle);
    }

}

void FileSort::writeNewChunk(const std::unique_ptr<char>& readBuffer, const int& chunkNumber) const
{
    HANDLE newChunkFile;
    newChunkFile = HelperUtils::openFile(std::format("chunk{}.txt", chunkNumber), 'w');
    WriteFile(newChunkFile, readBuffer.get(), _numberOfLinesPerSegment * _lineSizeBytes, NULL, NULL);
    CloseHandle(newChunkFile);
}

priority_queue FileSort::getFirstPQueue(std::vector<std::string>& chunks) const
{
    std::vector<wordData> preSortedValues;
    HANDLE chunkReadHandle;
    for (auto chunk = std::begin(chunks); chunk != std::end(chunks); ++chunk)
    {
        
        chunkReadHandle = HelperUtils::openFile(*chunk, 'r');
        std::unique_ptr<char> readBuffer(new char[_lineSizeBytes + 1 ]);
        if (!ReadFile(chunkReadHandle, static_cast<char*>(readBuffer.get()), _lineSizeBytes, NULL, NULL))
        {
            throw "Bla";
        }
        readBuffer.get()[_lineSizeBytes] = '\0';
        preSortedValues.push_back(std::pair(std::string(readBuffer.get()),*chunk));
        CloseHandle(chunkReadHandle);
        if (!removeFirstLine(*chunk))
        {
            chunks.erase(chunk);
            std::cout << *chunk << " is done.\n";
        }
    }
    priority_queue sortedQueue(std::begin(preSortedValues), std::end(preSortedValues));
    return sortedQueue;
}

wordData FileSort::getFirstLine(std::string chunk, std::vector<std::string>& chunks) const
{
    auto chunkReadHandle = HelperUtils::openFile(chunk, 'r');
    std::unique_ptr<char> readBuffer(new char[_lineSizeBytes + 1]);
    if (!ReadFile(chunkReadHandle, static_cast<char*>(readBuffer.get()), _lineSizeBytes, NULL, NULL))
    {
        throw "Bla";
    }
    readBuffer.get()[_lineSizeBytes] = '\0';
    CloseHandle(chunkReadHandle);
    if (!removeFirstLine(chunk))
    {
        chunks.erase(std::find(std::begin(chunks), std::end(chunks), chunk));
    }
    return std::pair(std::string(readBuffer.get()), chunk);
}

bool FileSort::removeFirstLine(std::string filePath) const
{
    HANDLE chunkReadHandle;
    try
    {
        chunkReadHandle = HelperUtils::openFile(filePath, 'r');
    }
    catch (std::exception& err)
    {
        std::cerr << err.what();
        return false;
    }
    if (GetFileSize(chunkReadHandle,NULL) == 0)
    {
        CloseHandle(chunkReadHandle);
        DeleteFileA(filePath.c_str());
        return false;
    }
    SetFilePointer(chunkReadHandle, _lineSizeBytes, NULL, FILE_BEGIN);
    auto newFileHanle = HelperUtils::openFile("temp.txt", 'w');
    std::unique_ptr<char> readBuffer(new char[ _lineSizeBytes + 1]);
    DWORD numberOfReadBytes = 0;
    while (ReadFile(chunkReadHandle, readBuffer.get(), _lineSizeBytes, &numberOfReadBytes, NULL))
    {
        if (numberOfReadBytes == 0)
            break;
        readBuffer.get()[_lineSizeBytes] = '\0';
        WriteFile(newFileHanle, readBuffer.get(), numberOfReadBytes, NULL, NULL);
    }
    CloseHandle(chunkReadHandle);
    CloseHandle(newFileHanle);
    DeleteFileA(filePath.c_str());
    rename("temp.txt", filePath.c_str());
    return true;
}
