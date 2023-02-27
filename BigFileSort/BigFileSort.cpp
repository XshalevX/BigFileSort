#include <iostream>
#include "FileSort.h"

int main()
{
    FileSort fs(30, 2, 5);
    try
    {
        fs.Sort("input.txt", "output.txt");
    }
    catch(std::exception& err)
    {
        std::cerr << err.what();
        return 1;
    }
}


