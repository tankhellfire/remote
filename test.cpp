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

    std::string meStr;

    //while(GetTickCount()-(static_cast<uint32_t>(*myMem))<2000){
    while(true){
        *static_cast<uint32_t*>(myMem) = GetTickCount();
        if(2000<(GetTickCount()-(static_cast<uint32_t>(*theirMem)))){

            //meStr=std::string(path)+std::string(" ")+std::to_string((me+1)%2);
            meStr=std::string("C:\\Users\\tt\\Downloads\\a.exe")+std::string(" ")+std::to_string((me+1)%2);
            ShellExecute(NULL, "open",meStr.c_str(), NULL, NULL, SW_SHOWNORMAL);
        }
        Sleep(1000);
    };
}