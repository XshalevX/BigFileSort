#include <iostream>
#include "FileSort.h"

int main()
{
    auto x = FileSort(500*12, 10, 12);
    try
    {
        x.Sort("input.txt", "output.txt");
    }
    catch(std::exception& err)
    {
        std::cerr << err.what();
        return 1;
    }
}


