#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>
#include <string>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "ws2_32.lib")

const char* responseHeader = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Connection: close\r\n"
    "\r\n";

const char* responseBody = 
    "<html>"
    "<head><title>Simple HTTP Server</title></head>"
    "<body><h1>Hello, World!</h1></body>"
    "</html>";

DWORD WINAPI ClientHandler(LPVOID clientSocket) {
    SOCKET client = *(SOCKET*)clientSocket;
    char buffer[1024];
    int bytesRead;

    // Read the request
    bytesRead = recv(client, buffer, sizeof(buffer), 0);
    if (bytesRead > 0) {
        // Log the request
        std::cout << "Received request:\n" << std::string(buffer, bytesRead) << std::endl;

        // Send the response
        send(client, responseHeader, strlen(responseHeader), 0);
        send(client, responseBody, strlen(responseBody), 0);
    }

    // Close the client socket
    closesocket(client);
    return 0;
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    HANDLE clientThread;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    // Bind the socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "bind failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port 8080..." << std::endl;

    // Accept and handle incoming connections
    while (true) {
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "accept failed" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        // Create a thread to handle the client
        clientThread = CreateThread(NULL, 0, ClientHandler, &clientSocket, 0, NULL);
        if (clientThread == NULL) {
            std::cerr << "CreateThread failed" << std::endl;
            closesocket(clientSocket);
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        // Close the thread handle as we don't need it
        CloseHandle(clientThread);
    }

    // Cleanup
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
