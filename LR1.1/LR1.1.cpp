#include "pch.h"
#define BUFFER_SIZE 200

void ErrorExit(LPTSTR lpszFunction)
{

    // Retrieve the system error message for the last-error code
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw);
}

void asciiToUnicode() {
    HANDLE firstFile = CreateFile(L"inFileAnci.txt",
        FILE_ALL_ACCESS,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    HANDLE secondFile = CreateFile(L"outFileUni.txt",
        FILE_ALL_ACCESS,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    LPSTR lpBuffer = new CHAR[BUFFER_SIZE];
    LPWSTR lpwBuffer = new WCHAR[BUFFER_SIZE];

    DWORD dwBytesRead = 0;

    do
    {
        ReadFile(firstFile,
            lpBuffer,
            BUFFER_SIZE,
            &dwBytesRead,
            NULL
        );

         dwBytesRead = MultiByteToWideChar(CP_ACP,
            0,
            lpBuffer,
            dwBytesRead,
            lpwBuffer,
            BUFFER_SIZE
        );

        WriteFile(secondFile,
            lpwBuffer,
            dwBytesRead,
            NULL,
            NULL);


    } while (dwBytesRead!=0);

    TCHAR functionName[] = TEXT("asciiToUnicode");
    if(GetLastError() != 0) ErrorExit(functionName);
}

void unicodeToAscii() {
    HANDLE firstFile = CreateFile(L"inFileUni.txt",
        FILE_ALL_ACCESS,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    HANDLE secondFile = CreateFile(L"outFileAnci.txt",
        FILE_ALL_ACCESS,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    LPSTR lpBuffer = new CHAR[BUFFER_SIZE];
    LPWSTR lpwBuffer = new WCHAR[BUFFER_SIZE];
    TCHAR functionName[] = TEXT("unicodeToAscii");
    DWORD dwBytesRead = 0;

    do
    {
        ReadFile(firstFile,
            lpwBuffer,
            BUFFER_SIZE * sizeof(WCHAR),
            &dwBytesRead,
            NULL
        );

        WideCharToMultiByte(CP_ACP,
            0,
            lpwBuffer,
            dwBytesRead / sizeof(WCHAR),
            lpBuffer,
            BUFFER_SIZE,
            NULL,
            NULL
        );

        WriteFile(secondFile,
            lpBuffer,
            dwBytesRead,
            NULL,
            NULL);
    } while (dwBytesRead != 0);

    if (GetLastError() != 0) ErrorExit(functionName);
}

void systemInfo() {
    SYSTEM_INFO siSysInfo;

    // Copy the hardware information to the SYSTEM_INFO structure. 

    GetSystemInfo(&siSysInfo);

    // Display the contents of the SYSTEM_INFO structure. 

    printf("Hardware information: \n");
    printf("  OEM ID: %u\n", siSysInfo.dwOemId);
    printf("  Number of processors: %u\n",
        siSysInfo.dwNumberOfProcessors);
    printf("  Page size: %u\n", siSysInfo.dwPageSize);
    printf("  Processor type: %u\n", siSysInfo.dwProcessorType);
    printf("  Minimum application address: %lx\n",
        siSysInfo.lpMinimumApplicationAddress);
    printf("  Maximum application address: %lx\n",
        siSysInfo.lpMaximumApplicationAddress);
    printf("  Active processor mask: %u\n",
        siSysInfo.dwActiveProcessorMask);
}

int main(int argc, char* argv[])
{
    
    if (argc == 2 && _stricmp(argv[1], "-e") == 0)
    {
        LocalAlloc(LHND, 90000000000);
        // Вызываем функцию ErrorExit для вывода описания ошибки 8
        TCHAR functionName[] = TEXT("createError");
        ErrorExit(functionName); // Замените "YourFunctionNameHere" на имя вашей функции
    }
    else if (argc == 2 && _stricmp(argv[1], "-s") == 0) {
        systemInfo();
    }
    else if (argc == 2 && _stricmp(argv[1], "-a") == 0) {
        asciiToUnicode();
    }
    else if (argc == 2 && _stricmp(argv[1], "-u") == 0) {
        unicodeToAscii();
    }
    return 0;
}
