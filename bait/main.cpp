#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>

int main(int argc, char* argv[])
{
    int me;
    if(argc<2){
        me=0;
    }else if(false){
        me=2;
    }else{
        me=std::stoi(argv[1]);
    }
    HANDLE shm_handle  = CreateFileMappingW(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,8,L"JrRKULBcTr");
    void* shm_ptr = MapViewOfFile(shm_handle,FILE_MAP_ALL_ACCESS,0,0,8);

    uint32_t* myMem = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(shm_ptr) + (me*4));
    uint32_t* theirMem = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(shm_ptr) + (((me+1)%2)*4));


    char path[MAX_PATH]="";
    char* meStr;

    GetModuleFileNameA(NULL, path, MAX_PATH);
    meStr=const_cast<char*>((std::string(path)+std::string(" ")+std::to_string((me+1)%2)).c_str());

    HANDLE file = CreateFile(meStr, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    uint32_t lastMyMem = 0;
    //while(GetTickCount()-(static_cast<uint32_t>(*myMem))<2000){
    while(true){
        lastMyMem = GetTickCount();
        *static_cast<uint32_t*>(myMem) = lastMyMem;
        if(200<(GetTickCount()-(static_cast<uint32_t>(*theirMem)))){

            STARTUPINFO startupInfo;
            PROCESS_INFORMATION processInfo;
            ZeroMemory(&startupInfo, sizeof(startupInfo));
            startupInfo.cb = sizeof(startupInfo);

            GetModuleFileNameA(NULL, path, MAX_PATH);

            meStr=const_cast<char*>((std::string(path)+std::string(" ")+std::to_string((me+1)%2)).c_str());

            //WriteFile(file, meStr, strlen(meStr), NULL, NULL);
            CreateProcessA(NULL, meStr, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);
        }
        Sleep(100);
        if((lastMyMem!=static_cast<uint32_t>(*myMem))&&(lastMyMem!=0)){
            while(true){}
        }
    };
}