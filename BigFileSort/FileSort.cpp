#include "FileSort.h"



FileSort::FileSort(unsigned long maxFileSizeBytes, int numberOfLinesPerSegment, int lineSizeBytes):
	_maxFileSizeBytes(maxFileSizeBytes), _numberOfLinesPerSegment(numberOfLinesPerSegment), _lineSizeBytes(lineSizeBytes) {}



/// <summary>
/// this function is the main function of the program
/// it will sort any given file that fit the requirements 
/// </summary>
/// <param name="inFilePath">path to unsorted file</param>
/// <param name="outFilePath">path to the new sorted file</param>
void FileSort::Sort(const std::string& inFilePath, const std::string& outFilePath) const
{
    int byteReadCounter = 0;
    int chunkCounter = 1;

    HANDLE hInfile;
    HANDLE finalSortedFile;
    std::vector<std::string> chunksData;


    hInfile = openFile(inFilePath,'r');
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
            CloseHandle(hInfile);
            throw ReadException(inFilePath);
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

        writeNewChunk(readBuffer, chunksData[chunksData.size() - 1]);
        

        chunkCounter++;
                
    }while(hInfile && numberOfBytesRead != 0);

    //delete it
    CloseHandle(hInfile);
    DeleteFileA(inFilePath.c_str());

    sortChunks(chunksData);
    

    finalSortedFile = openFile(outFilePath, 'w');
    auto queue = getFirstPQueue(chunksData);

    while (!chunksData.empty())
    {
        auto data = queue.top();
        if (!WriteFile(finalSortedFile, data.first.c_str(), data.first.size(), NULL, NULL))
        {
            throw WriteException(outFilePath);
        }
        auto newLine = getFirstLine(data.second, chunksData);
        queue.pop();
        queue.push(newLine);

    }
    

    

}

/// <summary>
/// this function sorts every chunk individually
/// </summary>
/// <param name="chunks">the path to all the active chunks</param>
void FileSort::sortChunks(std::vector<std::string>& chunks) const
{
    HANDLE chunkWriteHandle;
    HANDLE chunkReadHandle;
    for (auto chunkFilePath : chunks)
    {

        chunkReadHandle = openFile(chunkFilePath, 'r');
        std::unique_ptr<char> readBuffer(new char[_numberOfLinesPerSegment * _lineSizeBytes]);
        if (!ReadFile(chunkReadHandle, static_cast<char*>(readBuffer.get()), _numberOfLinesPerSegment * _lineSizeBytes, NULL, NULL))
        {
            throw ReadException(chunkFilePath);
        }
        auto lines = split(readBuffer.get(), _numberOfLinesPerSegment * _lineSizeBytes, '\n');
        std::sort(lines.begin(), lines.end());
        CloseHandle(chunkReadHandle);
        chunkWriteHandle = openFile(chunkFilePath, 'w');
        std::string combainedString = "";
        for (auto line : lines)
        {
            combainedString += line;
        }
        if (!WriteFile(chunkWriteHandle, combainedString.c_str(), _numberOfLinesPerSegment * _lineSizeBytes, NULL, NULL))
        {
            throw WriteException(chunkFilePath);
        }
        CloseHandle(chunkWriteHandle);
    }

}


/// <summary>
/// this function will write a new unsorted chunk to disk
/// </summary>
/// <param name="readBuffer">the data that will be inserted into readBuffer</param>
/// <param name="chunkPath">the path to the chunk</param>
void FileSort::writeNewChunk(const std::unique_ptr<char>& readBuffer, std::string chunkPath) const
{
    HANDLE newChunkFile;
    newChunkFile = openFile(chunkPath, 'w');
    if (!WriteFile(newChunkFile, readBuffer.get(), _numberOfLinesPerSegment * _lineSizeBytes, NULL, NULL))
    {
        throw WriteException(chunkPath);
    }
    CloseHandle(newChunkFile);
}


/// <summary>
/// this function gets the first puts all of the chunks's first line
/// into a priority queue
/// </summary>
/// <param name="chunks">the path to the chunks</param>
/// <returns>the priority queue with all the lines</returns>
priority_queue FileSort::getFirstPQueue(std::vector<std::string>& chunks) const
{
    std::vector<wordData> preSortedValues;
    HANDLE chunkReadHandle;
    for (auto chunk = std::begin(chunks); chunk != std::end(chunks); ++chunk)
    {
        
        chunkReadHandle = openFile(*chunk, 'r');
        std::unique_ptr<char> readBuffer(new char[_lineSizeBytes + 1 ]);
        if (!ReadFile(chunkReadHandle, static_cast<char*>(readBuffer.get()), _lineSizeBytes, NULL, NULL))
        {
            throw ReadException(*chunk);
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

/// <summary>
/// this function helps us to get the first line of a specific chunk
/// </summary>
/// <param name="chunk">the path to the chunk</param>
/// <param name="chunks">all the chunks, in case that the line that was
/// read was the last one we need to remove it</param>
/// <returns>a pair which hold the word and the chunk that it belonged to </returns>
wordData FileSort::getFirstLine(std::string chunk, std::vector<std::string>& chunks) const
{
    auto chunkReadHandle = openFile(chunk, 'r');
    std::unique_ptr<char> readBuffer(new char[_lineSizeBytes + 1]);
    if (!ReadFile(chunkReadHandle, static_cast<char*>(readBuffer.get()), _lineSizeBytes, NULL, NULL))
    {
        throw ReadException(chunk);
    }
    readBuffer.get()[_lineSizeBytes] = '\0';
    CloseHandle(chunkReadHandle);
    if (!removeFirstLine(chunk))
    {
        chunks.erase(std::find(std::begin(chunks), std::end(chunks), chunk));
    }
    return std::pair(std::string(readBuffer.get()), chunk);
}

/// <summary>
/// this function removes the first line of a given chunk
/// </summary>
/// <param name="filePath">the path to the chunk</param>
/// <returns>true if everything goes well and false if the chunk is empty</returns>
bool FileSort::removeFirstLine(std::string filePath) const
{
    HANDLE chunkReadHandle;
    HANDLE newFileHandle;
    DWORD numberOfReadBytes = 0;
    try
    {
        chunkReadHandle = openFile(filePath, 'r');
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
    newFileHandle = openFile("temp.txt", 'w');
    std::unique_ptr<char> readBuffer(new char[ _lineSizeBytes + 1]);
    while (ReadFile(chunkReadHandle, readBuffer.get(), _lineSizeBytes, &numberOfReadBytes, NULL))
    {
        if (numberOfReadBytes == 0)
            break;
        readBuffer.get()[_lineSizeBytes] = '\0';
        if (!WriteFile(newFileHandle, readBuffer.get(), numberOfReadBytes, NULL, NULL))
        {
            throw WriteException(std::string("temp.txt"));
        }
    }
    CloseHandle(chunkReadHandle);
    CloseHandle(newFileHandle);
    DeleteFileA(filePath.c_str());
    rename("temp.txt", filePath.c_str());
    return true;
}
