#include "InvaildInputExeption.h"

const char* InvaildInputExeption::what() const noexcept
{
    return "Invaild input\n";
}
