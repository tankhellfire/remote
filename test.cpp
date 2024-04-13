#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>

int main(int argc, char* argv[])
{
    HANDLE shm_handle  = CreateFileMappingW(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,8,L"JrRKULBcTr");
    void* shm_ptr = MapViewOfFile(shm_handle,FILE_MAP_ALL_ACCESS,0,0,8);

    uint32_t* myMem = static_cast<uint32_t*>(shm_ptr);
    uint32_t* theirMem = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(shm_ptr) + 4);

    char path[MAX_PATH]="";
    GetModuleFileName(NULL, path, MAX_PATH);

    //while(GetTickCount()-(static_cast<uint32_t>(*myMem))<2000){
    while(true){
        *static_cast<uint32_t*>(myMem) = GetTickCount();
        if(200<GetTickCount()-(static_cast<uint32_t>(*theirMem))){
            ShellExecute(NULL, "open",, NULL, NULL, SW_SHOWNORMAL);
        }
        Sleep(100);
    };
}