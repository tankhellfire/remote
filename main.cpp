#include <windows.h>
#include <shlwapi.h>

int main()
{
    ShellExecute(NULL, "open", "C:\\Windows\\System32\\calc.exe", NULL, NULL, SW_SHOWNORMAL);

    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    PathRemoveFileSpec(path);

    HANDLE file = CreateFile(strcat(path,"/bree bo bux.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
    
    WriteFile(file, "hello", strlen("hello"), NULL, NULL);
    CloseHandle(file);
}