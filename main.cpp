#include <windows.h>
#include <shlwapi.h>
#include <winhttp.h>
#include <string>

int main()
{
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    PathRemoveFileSpec(path);

    HINTERNET hSession = WinHttpOpen(L"User-Agent", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    HINTERNET hConnect = WinHttpOpenRequest(hSession, L"GET", L"https://chip-bronzed-plastic.glitch.me", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

    WinHttpSendRequest(hConnect, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)

    WinHttpReceiveResponse(hConnect, NULL)
    std::string responseData;
    DWORD bytesRead;
    char buffer[1024];
    while (WinHttpReadData(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        responseData += buffer;
    }

    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    HANDLE file = CreateFile(strcat(path,"/m.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(file, responseData, strlen(responseData), NULL, NULL);
}