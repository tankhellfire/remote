#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>

int main(int argc, char* argv[])
{
    HANDLE file = CreateFile("C:/Users/tt/Downloads/m.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(file, L"a\n",sizeof(wchar_t)*3, NULL, NULL);
    HANDLE shm_handle  = CreateFileMappingW(
    INVALID_HANDLE_VALUE,
    NULL,
    PAGE_READWRITE,
    0,
    4,
    L"main.exe");
    if (shm_handle  == NULL) {
        WriteFile(file, L"0\n", sizeof(wchar_t)*3, NULL, NULL);
        return 1;
    }

    void* shm_ptr = MapViewOfFile(
        shm_handle,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        4);

    if (shm_ptr == NULL) {
        WriteFile(file, L"1\n",sizeof(wchar_t)*3, NULL, NULL);
        CloseHandle(shm_handle);
        return 1;
    }
    char* shm_data = static_cast<char*>(shm_ptr);

    
    WriteFile(file, shm_data, strlen(shm_data), NULL, NULL);

    strcpy_s(shm_data, 4, 0x00000000);
}