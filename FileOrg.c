#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

//  Simple organizer built with love and some broken mouses (jk)
struct FileInfo
{
    WORD fileday;
    WORD filemonth;
    WORD fileyear;
};
typedef struct FileInfo FI;

int wmain(int argc, wchar_t *argv[])
{

    SYSTEMTIME st;
    WIN32_FIND_DATAW fileData;
    HANDLE hFind;

    if (argc != 2)
    {
        printf("Must Enter A <directory>\n");
        return 1;
    } // Check if the user provided a directory argument

    wchar_t searchPath[MAX_PATH];
    swprintf(searchPath, MAX_PATH, L"%ls\\*", argv[1]);
    // Create the main path by appending "\*" to the provided directory

    hFind = FindFirstFileW(searchPath, &fileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Could not open directory.\n");
        return 1;
    } // Check if the directory was opened successfully

    do
    {
        FileTimeToSystemTime(&fileData.ftCreationTime, &st);

        if (wcscmp(fileData.cFileName, L".") == 0 || wcscmp(fileData.cFileName, L"..") == 0)
        {
            continue; // Skip the current and parent directory entries
        }
        if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            continue;
        } // Skiop directories

        FI fileinfo;
        fileinfo.fileday = st.wDay;
        fileinfo.filemonth = st.wMonth;
        fileinfo.fileyear = st.wYear;

        wchar_t Dday[3];
        wchar_t Dmonth[3];
        wchar_t Dyear[5];

        swprintf(Dday, 3, L"%02d", fileinfo.fileday);
        swprintf(Dmonth, 3, L"%02d", fileinfo.filemonth);
        swprintf(Dyear, 5, L"%04d", fileinfo.fileyear);

        wchar_t Dirname[MAX_PATH];
        swprintf(Dirname, MAX_PATH, L"%ls-%ls-%ls", Dmonth, Dday, Dyear);

        wchar_t foldpath[MAX_PATH];
        wchar_t newpath[MAX_PATH];
        wchar_t newfpath[MAX_PATH];
        swprintf(foldpath, MAX_PATH, L"%ls\\%ls", argv[1], fileData.cFileName);              // C://myfolder//file
        swprintf(newpath, MAX_PATH, L"%ls\\%s", argv[1], Dirname);                           // C://myfolder//mm//dd//yyyy
        swprintf(newfpath, MAX_PATH, L"%ls\\%s\\%ls", argv[1], Dirname, fileData.cFileName); // C://myfolder//mm//dd//yyyy//file

        CreateDirectoryW(newpath, NULL);
        if (!MoveFileW(foldpath, newfpath))
        {
            printf("MoveFile failed. Error: %lu\n", GetLastError());
        };

        wprintf(L"file: %ls\n", fileData.cFileName);
        wprintf(L"DirName: %ls\n", Dirname);
        wprintf(L"foldpath: %ls\n", foldpath);
        wprintf(L"newpath: %ls\n", newpath);
        wprintf(L"newfpath: %ls\n", newfpath);
        // this is just to show everything working fine

    } while (FindNextFileW(hFind, &fileData));

    FindClose(hFind);
};