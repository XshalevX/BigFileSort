#include "WriteException.h"

WriteException::WriteException(std::string fileName) : _fileName(fileName)
{
}

const char* WriteException::what() const noexcept
{
    return std::format(R"(cannot write to file {})", _fileName).c_str();
}
