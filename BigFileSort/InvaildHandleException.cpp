#include "InvaildHandleException.h"

const char* InvaildHandleException::what() const noexcept
{
    return "Cannot create handle\n";
}
