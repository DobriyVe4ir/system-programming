// LR1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"
    
int main()
{
    //CreateFile();
    std::cout
        << "BOOL\t" << "|\t" << sizeof(BOOL) << "\t|\t" << MININT << "\t|\t" << MAXINT << "\r\n"
        << "INT\t" << "|\t" << sizeof(INT) << "\t|\t" << MININT << "\t|\t" << MAXINT << "\r\n"
        << "LONG\t" << "|\t" << sizeof(LONG) << "\t|\t" << MINLONG << "\t|\t" << MINLONG << "\r\n"
        << "BYTE\t" << "|\t" << sizeof(BYTE) << "\t|\t" << MIN_UCSCHAR << "\t|\t" << MAX_UCSCHAR << "\r\n"
        << "UINT\t" << "|\t" << sizeof(UINT) << "\t|\t" << 0 << "\t|\t" << UINT_MAX << "\r\n"
        << "ULONG\t" << "|\t" << sizeof(ULONG) << "\t|\t" << 0 << "\t|\t" << ULONG_MAX << "\r\n"
        << "WORD\t" << "|\t" << sizeof(WORD) << "\t|\t" << 0 << "\t|\t" << SHRT_MAX << "\r\n"
        << "DWORD\t" << "|\t" << sizeof(DWORD) << "\t|\t" << 0 << "\t|\t" << ULONG_MAX << "\r\n"
        << "FLOAT\t" << "|\t" << sizeof(FLOAT) << "\t|\t" << FLT_MIN << "\t|\t" << FLT_MAX << "\r\n"
        << "CHAR\t" << "|\t" << sizeof(CHAR) << "\t|\t" << CHAR_MIN << "\t|\t" << CHAR_MAX << "\r\n"
        << "WCHAR\t" << "|\t" << sizeof(WCHAR) << "\t|\t" << WCHAR_MIN << "\t|\t" << WCHAR_MAX << "\r\n";
}

