#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <vector>
#include <iostream>

int main(int argc, char* argv[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    CreateProcess(NULL, "C:\\Windows\\System32\\calc.exe 1", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
}