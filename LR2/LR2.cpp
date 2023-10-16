
#include "pch.h"

LPWSTR strToWStr(PCHAR str) {
    int length = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    LPWSTR wstr = new WCHAR[length];
    MultiByteToWideChar(CP_ACP, 0, str, -1, wstr, length);
    return wstr;
}

HANDLE strToHandle(PCHAR str) {
    int length = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    LPWSTR wStrSrc = new WCHAR[length];
    MultiByteToWideChar(CP_ACP, 0, str, -1, wStrSrc, length);
    return FindFirstFile(wStrSrc, NULL);
}

void printCatalog(PCHAR path) {
    WIN32_FIND_DATA FindFileData;
    
    LPSTR DirSpec = new CHAR[MAX_PATH + 1];  // directory specification
    DWORD dwError;

    strncpy(DirSpec, path, strlen(path) + 1);
    strncat(DirSpec, "\\*", 3);

    HANDLE hFind = strToHandle(DirSpec);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Invalid file handle. Error is %u\n", GetLastError());
        return;
    }
    else
    {
        _BY_HANDLE_FILE_INFORMATION lpFileInfo;
        

        while (FindNextFile(hFind, &FindFileData) != 0)
        {


            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                printf("<dir>  %ls\n", FindFileData.cFileName);
            }
            else
            {
                printf("<file> %ls\n", FindFileData.cFileName);
            }
        }

        dwError = GetLastError();
        FindClose(hFind);
        if (dwError != ERROR_NO_MORE_FILES)
        {
            printf("FindNextFile error. Error is %u\n", dwError);
            return;
        }
    }
}

void printDirName(PCHAR dir) {
    printf("Current directory is: %s\n", dir);
}

void info() {
    std::cout << "-cd <directory> Change directory\n";
    std::cout << "-ls Print dir list\n";
    std::cout << "-cp Copy file\n";
    std::cout << "-create <path> Create file\n";
    std::cout << "-del <path> Delete file\n";
    std::cout << "-info <name> Information file\n";
    std::cout << "-help print this text again\n";
}

void fileInfo(PCHAR path, PCHAR file) {
    PCHAR filePath = new CHAR(strlen(path) + strlen(file) + 2);
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA FindFileData;

    FILETIME FcreationTime;
    FILETIME FeditTime;
    SYSTEMTIME creationTime;
    SYSTEMTIME editTime;

    strcpy(filePath, path);
    if (path[strlen(path)-1] != '\\') {
        strcat(filePath, "\\");
    }
    strcat(filePath, file);

    int length = MultiByteToWideChar(CP_ACP, 0, filePath, -1, NULL, 0);
    LPWSTR WfilePath = new WCHAR[length];
    MultiByteToWideChar(CP_ACP, 0, filePath, -1, WfilePath, length);

    hFind = FindFirstFile(WfilePath, &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Invalid file handle. Error is %u\n", GetLastError());
        return;
    }


    FileTimeToSystemTime(&FindFileData.ftCreationTime, &creationTime);
    FileTimeToSystemTime(&FindFileData.ftCreationTime, &editTime);

    printf("\tFile name: \t%ls\n", FindFileData.cFileName);
    printf("\tCreation time: \n\t\t%04d-%02d-%02d %02d:%02d\n", creationTime.wYear, creationTime.wMonth, creationTime.wDay, creationTime.wHour, creationTime.wMinute);
    printf("\tEdit name: \n\t\t%04d-%02d-%02d %02d:%02d\n", editTime.wYear, editTime.wMonth, editTime.wDay, editTime.wHour, editTime.wMinute);
    printf("\tSize: \t%u\n", GetFileSize(hFind, NULL));

}

void copyFile(PCHAR strSrc, PCHAR strDest) {

    CopyFile(strToWStr(strSrc), strToWStr(strDest), NULL);
};

void deleteFile(PCHAR strSrc) {
    DeleteFile(strToWStr(strSrc));
}

void createFile(PCHAR strSrc) {
    CreateFile(strToWStr(strSrc), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}





int main(int argc, char* argv[])
{
    info();

    PCHAR curDir = new CHAR[MAX_PATH + 1];
    strncpy(curDir, "D:\\\0", 4);

    while(true) {

        printDirName(curDir);

        PCHAR argm = new CHAR[20];
        std::cin >> argm;
        #define STR_EQUALS(str1) (strcmp(str1, argm) == 0)
        
        //Сменить директорию+
        //Распечатать директорию+
        //Вывести подробную информацию о файле+
        //Скопировать файл+
        //Создать директорию
        //Удалить файл(пустую директорию)+
        

        if (STR_EQUALS("-cd")) {
            std::cin.ignore();
            std::cin.getline(curDir, MAX_PATH);
        }
        else if (STR_EQUALS("-ls")) {
            printCatalog(curDir);
        }
        else if (STR_EQUALS("-help")) {
            info();
        } 
        else if (STR_EQUALS("-info")) {
            PCHAR filename = new CHAR[41];

            std::cin.ignore();
            std::cin.getline(filename, 40);

            fileInfo(curDir, filename);
             
            delete[] filename;
        }
        else if (STR_EQUALS("-cp")) {
            PCHAR src = new CHAR[MAX_PATH];
            PCHAR dest = new CHAR[MAX_PATH];

            std::cout << "Source: \n";
            std::cin.ignore();
            std::cin.getline(src, MAX_PATH);

            std::cout << "Destination: \n";
            std::cin.getline(dest, MAX_PATH);
            
            copyFile(src, dest);

            delete[] src;
            delete[] dest;
        }
        else if (STR_EQUALS("-del")) {
            PCHAR dest = new CHAR[MAX_PATH];
            std::cin.ignore();
            std::cin.getline(dest, MAX_PATH);
            deleteFile(dest);

            delete[] dest;
        }
        else if (STR_EQUALS("-create")) {
            PCHAR dest = new CHAR[MAX_PATH];
            std::cin.ignore();
            std::cin.getline(dest, MAX_PATH);
            createFile(dest);

            delete[] dest;
        }
    }

    delete[] curDir;
    return (0);
}
