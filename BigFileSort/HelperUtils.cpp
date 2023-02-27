#include "HelperUtils.h"


/// <summary>
/// this function converts std::string into a type that is used in winapi
/// </summary>
/// <param name="originalStr"> string to convert</param>
/// <returns> a pointer to the new type</returns>
std::shared_ptr<wchar_t> getWindowsPstr(const std::string& originalStr)
{
    std::shared_ptr<wchar_t> wtext(new wchar_t[20]);
    mbstowcs_s(NULL, wtext.get(), originalStr.size() + 1 , originalStr.c_str(), originalStr.size());
    return wtext;
}


/// <summary>
/// Helper function to manage opeing file in a simpler way
/// </summary>
/// <param name="filePath">path to file to open</param>
/// <param name="readOrWrite"> 'w' for write or 'r' read </param>
/// <param name="openOption">another option that tells the function to open or create file</param>
/// <returns>The handle to the new file that we opened</returns>
HANDLE openFile(const std::string& filePath, char readOrWrite, int openOption)
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


/// <summary>
/// basic split function that splits ny a delimiter
/// </summary>
/// <param name="str">c_string to split</param>
/// <param name="size">the size of the c_string</param>
/// <param name="del">the delimiter</param>
/// <returns>a vector of the splited part</returns>
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

