#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>

int main(int argc, char* argv[])
{
    int me;
    if(argc<2){
        me=0;
    }else{
        me=std::stoi(argv[1]);
    }
    HANDLE shm_handle  = CreateFileMappingW(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,8,L"JrRKULBcTr");
    void* shm_ptr = MapViewOfFile(shm_handle,FILE_MAP_ALL_ACCESS,0,0,8);

    uint32_t* myMem = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(shm_ptr) + (me*4));
    uint32_t* theirMem = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(shm_ptr) + (((me+1)%2)*4));

    char path[MAX_PATH]="";
    GetModuleFileName(NULL, path, MAX_PATH);

    char* meStr=const_cast<char*>((std::string(path)+std::string(" ")+std::to_string((me+1)%2)).c_str());;
    HANDLE file = CreateFile(meStr, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    //while(GetTickCount()-(static_cast<uint32_t>(*myMem))<2000){
    while(true){
        *static_cast<uint32_t*>(myMem) = GetTickCount();
        if(200<(GetTickCount()-(static_cast<uint32_t>(*theirMem)))){

            STARTUPINFO startupInfo;
            PROCESS_INFORMATION processInfo;
            ZeroMemory(&startupInfo, sizeof(startupInfo));
            startupInfo.cb = sizeof(startupInfo);

            //WriteFile(file, meStr, strlen(meStr), NULL, NULL);
            CreateProcessA(NULL, meStr, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);
        }
        Sleep(100);
    };
}