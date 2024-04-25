#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>

DWORD GetParentProcessId() {
    DWORD parentProcessId = 0;

    // Take a snapshot of all processes in the system
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        // Get the first process in the snapshot
        if (Process32First(hSnapshot, &processEntry)) {
            DWORD currentProcessId = GetCurrentProcessId();

            // Iterate through the processes until we find the current process
            do {
                if (processEntry.th32ProcessID == currentProcessId) {
                    parentProcessId = processEntry.th32ParentProcessID;
                    break;
                }
            } while (Process32Next(hSnapshot, &processEntry));
        }

        CloseHandle(hSnapshot);
    }

    return parentProcessId;
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
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    DWORD thisPid = GetCurrentProcessId();
    DWORD otherPid = 0;

    const  char* id="\n";

    if(false){
        HANDLE file = CreateFile("C:/Users/tt/Downloads/nani", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
        CloseHandle(file);
    }else if(argc<2){
        id="\n<user>";

        STARTUPINFO startupInfo;
        PROCESS_INFORMATION processInfo;
        ZeroMemory(&startupInfo, sizeof(startupInfo));
        startupInfo.cb = sizeof(startupInfo);

        CreateProcessA(NULL, const_cast<char*>((std::string(path)+std::string(" whyAreYouReadingThis")).c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);
        otherPid = processInfo.dwProcessId;

        HANDLE file = CreateFile("C:/Users/tt/Downloads/user", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
        CloseHandle(file);
    }else{
        id="\n<parent>";
        DWORD otherPid = GetParentProcessId();

        HANDLE file = CreateFile("C:/Users/tt/Downloads/parent", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
        CloseHandle(file);
    }

    HANDLE file = CreateFile("C:/Users/tt/Downloads/log", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);

    while (isProcessRunning(otherPid)) {
        waitForProcessToExit(otherPid);
    }
    WriteFile(file, id+const_cast<char*>(otherPid), strlen(id+const_cast<char*>(otherPid)), NULL, NULL);
    CloseHandle(file);
    while(true){

    }
}