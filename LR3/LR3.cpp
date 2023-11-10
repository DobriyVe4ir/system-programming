#include "pch.h"
#define STR_EQUALS(str1) (strcmp(str1, argm) == 0)
#define STRING_LEN FILENAME_MAX

int main()
{
	printf_s("-zip [archive name] [<file names>]\n");
	printf_s("-unzip [archive name]\n");

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	HANDLE pipeR, pipeW;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdOutput = pipeW;
	si.hStdError = pipeW;


	PCHAR argm = new CHAR[20];
	while (true)
	{
		std::cin >> argm;
		if (STR_EQUALS("-zip")) {
			//-zip archive1 pch.h 7za.dll
			LPWSTR command = const_cast<LPWSTR>(TEXT("7za.exe a "));
			LPWSTR wstr = new WCHAR[STRING_LEN];
			std::wcin.ignore();
			std::wcin.getline(wstr, STRING_LEN);

			LPWSTR fullCommand = new WCHAR[lstrlen(command) + lstrlen(wstr) + 1];

			lstrcpy(fullCommand, command);
			wcscat_s(fullCommand, lstrlen(command) + lstrlen(wstr) + 1, wstr);
			


			//7za.exe a + wstr
			if (!CreateProcess(NULL,   // No module name (use command line)
				fullCommand,      // Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi)           // Pointer to PROCESS_INFORMATION structure
				)
			{
				printf("CreateProcess failed (%d).\n", GetLastError());
			}
		}
		else if (STR_EQUALS("-unzip")) {
			LPWSTR command = const_cast<LPWSTR>(TEXT("7za.exe e "));
			LPWSTR wstr = new WCHAR[STRING_LEN];
			std::wcin.ignore();
			std::wcin.getline(wstr, STRING_LEN);

			LPWSTR fullCommand = new WCHAR[lstrlen(command) + lstrlen(wstr) + 1];

			lstrcpy(fullCommand, command);
			wcscat_s(fullCommand, lstrlen(command) + lstrlen(wstr) + 1, wstr);

			//7za.exe e archive.7z
			if (!CreateProcess(NULL,   // No module name (use command line)
				fullCommand,      // Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi)           // Pointer to PROCESS_INFORMATION structure
				)
			{
				printf("CreateProcess failed (%d).\n", GetLastError());
			}
		}
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

	}
}