#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>

int main(int argc, char* argv[])
{
    HANDLE shm_handle  = CreateFileMappingW(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,8,L"JrRKULBcTr");
    void* shm_ptr = MapViewOfFile(shm_handle,FILE_MAP_ALL_ACCESS,0,0,8);

    uint32_t* a = static_cast<uint32_t*>(shm_ptr);
    uint32_t* shm_data = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(shm_ptr) + 4);
    
    *static_cast<uint32_t*>(shm_data) = GetTickCount();
    

    while(GetTickCount()-(static_cast<uint32_t>(*shm_data))<2000){
        Sleep(100);
        *static_cast<uint32_t*>(shm_data) = GetTickCount();
    };
}