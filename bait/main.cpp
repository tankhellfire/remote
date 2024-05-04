#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>
#include <string>

std::string fetchUrl(const std::string& url) {
    std::string response;
    HINTERNET hInternet, hURL;

    // Initialize WinINet
    hInternet = InternetOpen(L"Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet == NULL) {
        //Failed to initialize WinINet
        return "";
    }

    // Open the URL
    hURL = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hURL == NULL) {
        //Failed to open URL
        InternetCloseHandle(hInternet);
        return "";
    }

    // Read the response data
    const DWORD bufferSize = 4096;
    char buffer[bufferSize];
    DWORD bytesRead;
    do {
        if (!InternetReadFile(hURL, buffer, bufferSize - 1, &bytesRead)) {
            //Failed to read from URL
            InternetCloseHandle(hURL);
            InternetCloseHandle(hInternet);
            return "";
        }
        buffer[bytesRead] = '\0';
        response.append(buffer);
    } while (0 < bytesRead);

    // Clean up
    InternetCloseHandle(hURL);
    InternetCloseHandle(hInternet);

    return response;
}

int main(int argc, char* argv[])
{
    int me;
    if(argc<2){
        me=0;
    }else if(argv[1]=="update"){
        fetchUrl()
    }else{
        me=std::stoi(argv[1]);
    }
    HANDLE shm_handle  = CreateFileMappingW(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,8,L"JrRKULBcTr");
    void* shm_ptr = MapViewOfFile(shm_handle,FILE_MAP_ALL_ACCESS,0,0,8);

    uint32_t* myMem = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(shm_ptr) + (me*4));
    uint32_t* theirMem = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(shm_ptr) + (((me+1)%2)*4));


    char path[MAX_PATH]="";

    GetModuleFileNameA(NULL, path, MAX_PATH);

    HANDLE file = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    char* meStr=const_cast<char*>((std::string(path)+std::string(" ")+std::to_string((me+1)%2)).c_str());
    uint32_t lastMyMem = 0;
    //while(GetTickCount()-(static_cast<uint32_t>(*myMem))<2000){
    while(true){
        lastMyMem = GetTickCount();
        *static_cast<uint32_t*>(myMem) = lastMyMem;
        if(100<(GetTickCount()-(static_cast<uint32_t>(*theirMem)))){

            STARTUPINFO startupInfo;
            PROCESS_INFORMATION processInfo;
            ZeroMemory(&startupInfo, sizeof(startupInfo));
            startupInfo.cb = sizeof(startupInfo);

            //WriteFile(file, meStr, strlen(meStr), NULL, NULL);
            CreateProcessA(NULL, meStr, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);
        }
        Sleep(50);
        if((lastMyMem!=static_cast<uint32_t>(*myMem))&&(lastMyMem!=0)){
            return 0;
        }
    };
}