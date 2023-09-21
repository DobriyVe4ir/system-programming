// LR1.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
int main(int argc, char* argv[])
{
    std::cout << (sizeof(argv[1]));
    
    if (argv[1] == "-e") {
        std::cout << 123;
        LocalAlloc(LHND, 9000000);
        std::cout << GetLastError();
    }
    return 0;
}