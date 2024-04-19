# file

## get path
### my
char path[MAX_PATH];
GetModuleFileName(NULL, path, MAX_PATH);

### parent
char path[MAX_PATH];
GetModuleFileName(NULL, path, MAX_PATH);
PathRemoveFileSpec(path);

## run
CreateProcess(NULL, "C:\\Windows\\System32\\calc.exe 1", NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);

## make
HANDLE file = CreateFile("C:/Users/tt/Downloads/m.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);

## open
HANDLE file = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

## write    
WriteFile(file, "hello", strlen("hello"), NULL, NULL);

## close
CloseHandle(file);


# http
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