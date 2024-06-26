# file

## get path

### my
char path[MAX_PATH];
GetModuleFileName(NULL, path, MAX_PATH);

### parent
char path[MAX_PATH];
GetModuleFileName(NULL, path, MAX_PATH);
PathRemoveFileSpec(path);

### from pid
```cpp
std::string GetProcessPathFromPID(DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (hProcess) {
        char buffer[MAX_PATH] = { 0 };
        DWORD bufferSize = MAX_PATH;
        if (QueryFullProcessImageNameA(hProcess, 0, buffer, &bufferSize)) {
            CloseHandle(hProcess);
            return std::string(buffer);
        }
        CloseHandle(hProcess);
    }
    return std::string();
}
```

## run
```cpp
STARTUPINFO startupInfo;
PROCESS_INFORMATION processInfo;
ZeroMemory(&startupInfo, sizeof(startupInfo));
startupInfo.cb = sizeof(startupInfo);

CreateProcessA(NULL, "C:\\Windows\\System32\\calc.exe 1", NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);
```

## make
```cpp
HANDLE file = CreateFile("C:/Users/tt/Downloads/m.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
```

## open
HANDLE file = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

## read
```cpp
DWORD fileSize = GetFileSize(file, NULL);
char* buffer = new char[fileSize + 1];
DWORD bytesRead;
ReadFile(file, buffer, fileSize, &bytesRead, NULL);
buffer[fileSize] = '\0';
```

## write    
WriteFile(file, "hello", strlen("hello"), NULL, NULL);

## close
CloseHandle(file);


# http
```cpp
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
```
```cpp
std::string fetchUrl(const std::string& url) {
    std::string response;
    HINTERNET hInternet, hURL;

    // Initialize WinINet
    hInternet = InternetOpen(L"Mozilla/5.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet == NULL) {
        std::cerr << "Failed to initialize WinINet" << std::endl;
        return "";
    }

    // Open the URL
    hURL = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hURL == NULL) {
        std::cerr << "Failed to open URL: " << url << std::endl;
        InternetCloseHandle(hInternet);
        return "";
    }

    // Read the response data
    const DWORD bufferSize = 4096;
    char buffer[bufferSize];
    DWORD bytesRead;
    do {
        if (!InternetReadFile(hURL, buffer, bufferSize - 1, &bytesRead)) {
            std::cerr << "Failed to read from URL" << std::endl;
            InternetCloseHandle(hURL);
            InternetCloseHandle(hInternet);
            return "";
        }
        buffer[bytesRead] = '\0';
        response.append(buffer);
    } while (bytesRead > 0);

    // Clean up
    InternetCloseHandle(hURL);
    InternetCloseHandle(hInternet);

    return response;
}
```

# pid

## get this pid
```cpp
DWORD currentPid = GetCurrentProcessId();
```


## get child pid
```cpp
STARTUPINFO startupInfo;
PROCESS_INFORMATION processInfo;
ZeroMemory(&startupInfo, sizeof(startupInfo));
startupInfo.cb = sizeof(startupInfo);

CreateProcessA(NULL, "C:\\Windows\\System32\\calc.exe", NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);

DWORD childPid = processInfo.dwProcessId;
```

## is pid running
```cpp
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
```