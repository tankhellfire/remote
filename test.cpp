#include <iostream>
#include <cstdlib>
#include <windows.h>

// Global variables for shared memory and semaphore
HANDLE sharedMemory = NULL;
HANDLE semaphore = NULL;
int* count = NULL;

// Signal handler for termination signals
BOOL WINAPI console_handler(DWORD signal)
{
    if (signal == CTRL_C_EVENT || signal == CTRL_BREAK_EVENT) {
        // Decrement the shared count
        WaitForSingleObject(semaphore, INFINITE);
        (*count)--;
        ReleaseSemaphore(semaphore, 1, NULL);

        std::cout << "Instance forcefully closed. Running instances: " << *count << std::endl;

        // Unmap shared memory and close handles if this is the last instance
        if (*count == 0) {
            UnmapViewOfFile(count);
            CloseHandle(sharedMemory);
            CloseHandle(semaphore);
        }

        return TRUE;
    }

    return FALSE;
}

int main()
{
    // Create or open shared memory
    sharedMemory = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "UniqueApplicationName");
    if (sharedMemory == NULL) {
        sharedMemory = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int), "UniqueApplicationName");
        if (sharedMemory == NULL) {
            std::cerr << "Failed to create shared memory" << std::endl;
            return 1;
        }
    }

    // Map shared memory
    count = (int*)MapViewOfFile(sharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int));
    if (count == NULL) {
        std::cerr << "Failed to map shared memory" << std::endl;
        CloseHandle(sharedMemory);
        return 1;
    }

    // Create or open semaphore
    semaphore = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, "UniqueApplicationName");
    if (semaphore == NULL) {
        semaphore = CreateSemaphoreA(NULL, 1, 1, "UniqueApplicationName");
        if (semaphore == NULL) {
            std::cerr << "Failed to create semaphore" << std::endl;
            UnmapViewOfFile(count);
            CloseHandle(sharedMemory);
            return 1;
        }
    }

    // Initialize count if this is the first instance
    if (*count == 0) {
        *count = 1;
    } else {
        // Increment the shared count
        WaitForSingleObject(semaphore, INFINITE);
        (*count)++;
        ReleaseSemaphore(semaphore, 1, NULL);
    }

    std::cout << "Running instances: " << *count << std::endl;

    // Set console control handler for termination signals
    SetConsoleCtrlHandler(console_handler, TRUE);

    // Keep the program running
    while (true) {
        // Your program logic here
    }

    return 0;
}