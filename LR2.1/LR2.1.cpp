#include "pch.h"
#define STRING_LEN 80
struct HEADER {
    DWORD size;
    DWORD count;
};

struct RECORD {
	DWORD id;
	FILETIME date;
	CHAR data[80];
	DWORD redacted;
};

void info() {
	std::cout << "-d <number> delete record\n";
	std::cout << "-a <text> add record\n";
	std::cout << "-c <text> change record\n";
	std::cout << "-s <number> show record\n";
}

void add(PCHAR text) {
	HANDLE hFile = CreateFile(L"1.txt", GENERIC_ALL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	HEADER header;
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	ReadFile(hFile, &header, sizeof(header), NULL, NULL);
	
	FILETIME currentTime;
	GetSystemTimeAsFileTime(&currentTime);

	RECORD record;

	record.id = header.count + 1;
	record.redacted = 0;
	record.date = currentTime;
	strncpy_s(record.data, text, 80);

	SetFilePointer(hFile, sizeof(header)+header.count*sizeof(record), NULL, FILE_BEGIN);

	WriteFile(hFile, &record, sizeof(record), NULL, NULL);

	header.count++;
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	WriteFile(hFile, &header, sizeof(header), NULL, NULL);

	CloseHandle(hFile);
}

void show(DWORD recordID) {
	HANDLE hFile = CreateFile(L"1.txt", GENERIC_ALL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HEADER header;
	RECORD record;
	SYSTEMTIME creationTime;

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	ReadFile(hFile, &header, sizeof(header), NULL, NULL);

	SetFilePointer(hFile, sizeof(header) + (recordID-1) * sizeof(RECORD), NULL, FILE_BEGIN);
	ReadFile(hFile, &record, sizeof(RECORD), NULL, NULL);

	
	FileTimeToSystemTime(&record.date, &creationTime);

	printf_s("Total info:\n");
	printf_s("%d records, %d bytes\n", header.count, header.size);
	printf_s("____________________\n");


	printf_s("id: %d\n", record.id);
	printf_s("Creation time: %04d-%02d-%02d %02d:%02d\n", creationTime.wYear, creationTime.wMonth, creationTime.wDay, creationTime.wHour, creationTime.wMinute);
	printf_s("data: \n\t%s\n", record.data);
	printf_s("redacted: %d\n", record.redacted);
	printf_s("____________________\n");

	CloseHandle(hFile);
}

void deleteRecord(DWORD recordID) {
	HANDLE hFile = CreateFile(L"1.txt", GENERIC_ALL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HEADER header;
	
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	ReadFile(hFile, &header, sizeof(header), NULL, NULL);

	if (recordID > 0 && recordID <= header.count) {
		for (DWORD i = recordID; i < header.count; i++) {
			RECORD currentRecord;
			SetFilePointer(hFile, sizeof(header) + (i) * sizeof(RECORD), NULL, FILE_BEGIN);
			ReadFile(hFile, &currentRecord, sizeof(currentRecord), NULL, NULL);
			currentRecord.id--;
			SetFilePointer(hFile, -(INT)(sizeof(currentRecord))*2, NULL, FILE_CURRENT);
			WriteFile(hFile, &currentRecord, sizeof(currentRecord), NULL, NULL);
		}	
		SetEndOfFile(hFile);
		header.count--;
		SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
		WriteFile(hFile, &header, sizeof(header), NULL, NULL);		
	}
	CloseHandle(hFile);
}

void changeRecord(DWORD recordID, PCHAR record) {
	HANDLE hFile = CreateFile(L"1.txt", GENERIC_ALL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HEADER header;
	RECORD currentRecord;
	
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	ReadFile(hFile, &header, sizeof(header), NULL, NULL);

	if (recordID > 0 && recordID <= header.count) {
		
		SetFilePointer(hFile, sizeof(header) + (recordID - 1) * sizeof(RECORD), NULL, FILE_BEGIN);
		ReadFile(hFile, &currentRecord, sizeof(currentRecord), NULL, NULL);

		strcpy_s(currentRecord.data, record);
		currentRecord.redacted++;

		SetFilePointer(hFile, -(INT)sizeof(RECORD), NULL, FILE_CURRENT);
		
		WriteFile(hFile, &currentRecord, sizeof(RECORD), NULL, NULL);
	}

	CloseHandle(hFile);
}


void init() {
	HANDLE hFile = CreateFile(L"1.txt", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HEADER header = { 0, 0 };
	BYTE* buffer = reinterpret_cast<BYTE*>(&header);
	WriteFile(hFile, buffer, sizeof(header), NULL, NULL);
	CloseHandle(hFile);
}

int main()
{
	info();

	init();
	PCHAR argm = new CHAR[20];

	while (true)
	{
		
		std::cin >> argm;
		#define STR_EQUALS(str1) (strcmp(str1, argm) == 0)


		if (STR_EQUALS("-a")) {
			PCHAR src = new CHAR[STRING_LEN];
			std::cin.ignore();
			std::cin.getline(src, STRING_LEN);
			add(src);
		} 
		else if (STR_EQUALS("-s")) {
			PCHAR src = new CHAR[10];
			std::cin.ignore();
			std::cin.getline(src, 10);
			show(std::atoi(src));
		} 
		else if (STR_EQUALS("-d")) {
			PCHAR src = new CHAR[10];
			std::cin.ignore();
			std::cin.getline(src, 10);
			deleteRecord(std::atoi(src));
		} 
		else if (STR_EQUALS("-c")) {
			PCHAR src = new CHAR[10];
			std::cin.ignore();
			std::cin.getline(src, 10);

			PCHAR str = new CHAR[STRING_LEN];
			std::cout << "Text: \n";
			//std::cin.ignore();
			std::cin.getline(str, STRING_LEN);

			changeRecord(std::atoi(src), str);
		}
 	}

}
