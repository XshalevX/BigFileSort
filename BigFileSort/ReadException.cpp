#include "ReadException.h"

ReadException::ReadException(std::string fileName) : _fileName(fileName)
{
}

const char* ReadException::what() const noexcept
{
    return std::format(R"(cannot read from file {})", _fileName).c_str();
}
