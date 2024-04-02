#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>

int main(int argc, char* argv[])
{
    HANDLE shm_handle  = CreateFileMappingW(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,4,L"main.exe");

    void* shm_ptr = MapViewOfFile(shm_handle,FILE_MAP_ALL_ACCESS,0,0,4);
    char* shm_data = static_cast<char*>(shm_ptr);
    

    *reinterpret_cast<uint32_t*>(shm_data) = 0x11121314;
    while(true){};
}