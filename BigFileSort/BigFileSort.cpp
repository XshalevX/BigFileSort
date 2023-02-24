#include <iostream>
#include "FileSort.h"

int main()
{
    FileSort fs(30, 2, 5);
    fs.Sort("input.txt", "");
    std::cout << "Hello World!\n";
}


