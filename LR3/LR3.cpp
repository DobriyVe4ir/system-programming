#include "pch.h"
#include <windows.h>
#include <tchar.h>
#include <iostream>

#define STR_EQUALS(str1) (strcmp(str1, argm) == 0)
#define STRING_LEN FILENAME_MAX

void CreatePipes(HANDLE& hReadPipe, HANDLE& hWritePipe) {
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0);
    SetHandleInformation(hWritePipe, HANDLE_FLAG_INHERIT, 0);
}

int main() {
    while (true) {
        printf_s("-zip [archive name] [<file names>]\n");
        printf_s("-unzip [archive name]\n");

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        HANDLE pipeRead, pipeWrite;

        CreatePipes(pipeRead, pipeWrite);

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdOutput = pipeWrite;
        si.hStdError = pipeWrite;

        PCHAR argm = new CHAR[20];

        std::cin >> argm;
        LPWSTR command;

        if (STR_EQUALS("-zip")) {
            command = const_cast<LPWSTR>(TEXT("7za.exe a "));
        }
        else if (STR_EQUALS("-unzip")) {
            command = const_cast<LPWSTR>(TEXT("7za.exe e "));
        }
        else {
            std::cerr << "Invalid command. Exiting." << std::endl;
            break;
        }

        LPWSTR wstr = new WCHAR[STRING_LEN];

        std::wcin.ignore();
        std::wcin.getline(wstr, STRING_LEN);

        LPWSTR fullCommand = new WCHAR[lstrlen(command) + lstrlen(wstr) + 1];

        lstrcpy(fullCommand, command);
        wcscat_s(fullCommand, lstrlen(command) + lstrlen(wstr) + 1, wstr);

        if (!CreateProcess(NULL, fullCommand, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            std::cerr << "CreateProcess failed (" << GetLastError() << ")." << std::endl;
            break;
        }

        CloseHandle(pipeWrite);

        char buffer[4096];
        DWORD bytesRead;

        while (ReadFile(pipeRead, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead != 0) {
            std::cout.write(buffer, bytesRead);
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(pipeRead);

        delete[] argm;
        delete[] wstr;
        delete[] fullCommand;
    }

    return 0;
}
