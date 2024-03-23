#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>
#include <string>

#pragma comment(lib, "wininet.lib")

int main()
{
    // Execute calc.exe
    ShellExecute(NULL, "open", "C:\\Windows\\System32\\calc.exe", NULL, NULL, SW_SHOWNORMAL);

    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    PathRemoveFileSpec(path);

    // Create file
    HANDLE file = CreateFile(strcat(path, "/bree_bo_bux.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file != INVALID_HANDLE_VALUE)
    {
        HINTERNET hInternet = InternetOpen("UserAgent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

        if (hInternet)
        {
            HINTERNET hConnect = InternetOpenUrl(hInternet, "https://mica-obsidian-sousaphone.glitch.me/", NULL, 0, INTERNET_FLAG_RELOAD, 0);

            if (hConnect)
            {
                char buffer[1024];
                DWORD bytesRead;
                
                while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead != 0)
                {
                    buffer[bytesRead] = '\0';
                    WriteFile(file, buffer, bytesRead, NULL, NULL);
                }

                InternetCloseHandle(hConnect);
            }

            InternetCloseHandle(hInternet);
        }

        CloseHandle(file);
    }

    return 0;
}
