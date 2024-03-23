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
ShellExecute(NULL, "open", "C:\\Windows\\System32\\calc.exe", NULL, NULL, SW_SHOWNORMAL);

## make
HANDLE file = CreateFile("C:/Users/tt/Downloads/m.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);

## open
file = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

## write    
WriteFile(file, "hello", strlen("hello"), NULL, NULL);

## close
CloseHandle(file);


# http