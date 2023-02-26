#include "HelperUtils.h"

std::shared_ptr<wchar_t> HelperUtils::getWindowsPstr(const std::string& originalStr)
{
    std::shared_ptr<wchar_t> wtext(new wchar_t[20]);
    mbstowcs_s(NULL, wtext.get(), originalStr.size() + 1 , originalStr.c_str(), originalStr.size());
    return wtext;
}



HANDLE HelperUtils::openFile(const std::string& filePath, char readOrWrite, int openOption)
{
    if (readOrWrite == READ)
    {
        openOption = openOption ? openOption : OPEN_EXISTING;
        auto readHandle = CreateFile(getWindowsPstr(filePath).get(),
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (readHandle == INVALID_HANDLE_VALUE)
        {
            throw InvaildHandleException();
        }
        return readHandle;
    }
    else if (readOrWrite == WRITE)
    {
        openOption = openOption ? openOption : CREATE_ALWAYS;

        auto writeHandle = CreateFile(getWindowsPstr(filePath).get(),
            GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (writeHandle == INVALID_HANDLE_VALUE)
        {
            throw InvaildHandleException();
        }
        return writeHandle;
    }
    else if (readOrWrite == APPEND)
    {
        openOption = openOption ? openOption : CREATE_ALWAYS;

        auto append = CreateFile(getWindowsPstr(filePath).get(),
            GENERIC_WRITE,
            FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (append == INVALID_HANDLE_VALUE)
        {
            throw InvaildHandleException();
        }
        return append;
    }
    else
    {
        throw InvaildInputExeption();
    }
}

std::vector<std::string> HelperUtils::createChunksName(int numberOfChunks)
{
    std::vector<std::string> chunksFilePath;
    for (int i = 1; i < numberOfChunks; i++)
    {
        chunksFilePath.push_back(std::format("chunk{}.txt", i));
    }
    return chunksFilePath;
}
std::vector<std::string> split(const char* str, const int size, const char del)
{
    std::string temp = "";
    std::vector<std::string> lines = std::vector<std::string>();
    for (int i = 0; i < size; i++)
    {
        if (str[i] == del)
        {
            lines.push_back(temp + del);
            temp = "";
        }
        else
        {
            temp += str[i];
        }
    }
    return lines;
}

