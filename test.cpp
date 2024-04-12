#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>

int main(int argc, char* argv[])
{
    HANDLE shm_handle  = CreateFileMappingW(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,4,L"main.exe");

    void* shm_ptr = MapViewOfFile(shm_handle,FILE_MAP_ALL_ACCESS,0,0,4);
    char* shm_data = static_cast<char*>(shm_ptr);
    
    *reinterpret_cast<uint32_t*>(shm_data) = GetTickCount();

    HANDLE file = CreateFile("C:/Users/tt/Downloads/m.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);

    std::string utf8_str = std::to_string(*shm_data) + std::string(" , ") + std::to_string(GetTickCount());
    
    WriteFile(file, utf8_str.c_str(), static_cast<DWORD>(utf8_str.length()), NULL, NULL);

    while(GetTickCount()-*shm_data<20000){};
}