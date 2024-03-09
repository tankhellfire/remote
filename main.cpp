#include <windows.h>
int main()
{
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);  
}