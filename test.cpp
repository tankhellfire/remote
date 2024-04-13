#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>

int main(int argc, char* argv[])
{
    int me;
    if(argc<2){
        int me=0;
    }else{
        int me=std::stoi(argv[1]);
    }
    HANDLE shm_handle  = CreateFileMappingW(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,8,L"JrRKULBcTr");
    void* shm_ptr = MapViewOfFile(shm_handle,FILE_MAP_ALL_ACCESS,0,0,8);

    uint32_t* myMem = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(shm_ptr) + (me*4));
    uint32_t* theirMem = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(shm_ptr) + (((me+1)%2)*4));

    char path[MAX_PATH]="";
    GetModuleFileName(NULL, path, MAX_PATH);

    HANDLE file = CreateFile("C:/Users/tt/Downloads/m.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
    std::string meStr = std::to_string(me);
    WriteFile(file, meStr.c_str(), strlen(meStr.c_str()), NULL, NULL);

    //while(GetTickCount()-(static_cast<uint32_t>(*myMem))<2000){
    while(true){
        *static_cast<uint32_t*>(myMem) = GetTickCount();
        if(200<GetTickCount()-(static_cast<uint32_t>(*theirMem))){
            ShellExecute(NULL, "open",(std::string(path)+std::string(" ")+std::to_string((me+1)%2)).c_str(), NULL, NULL, SW_SHOWNORMAL);
        }
        Sleep(100);
    };
}