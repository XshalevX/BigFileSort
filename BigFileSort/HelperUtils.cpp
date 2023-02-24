#include "HelperUtils.h"

std::shared_ptr<wchar_t> HelperUtils::getWindowsPstr(const std::string& originalStr)
{
    std::shared_ptr<wchar_t> wtext(new wchar_t[20]);
    mbstowcs_s(NULL, wtext.get(), originalStr.size() + 1 , originalStr.c_str(), originalStr.size());
    return wtext;
}



HANDLE HelperUtils::openFile(const std::string& filePath, char readOrWrite)
{
    if (readOrWrite == READ)
    {
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
    else
    {
        throw InvaildInputExeption();
    }
}
