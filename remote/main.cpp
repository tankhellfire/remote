#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <shlwapi.h>
#include <wininet.h>
#include <iostream>
#include <string>
#include <sstream>

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

struct HttpRequest {
    std::string method;
    std::string path;
    char* body;
    size_t bodyLength;
};

HttpRequest parseRequest(char* request, size_t length) {
    HttpRequest httpRequest;
    std::istringstream requestStream(std::string(request, length));
    std::string line;

    // Parse the request line
    std::getline(requestStream, line);
    std::istringstream lineStream(line);
    lineStream >> httpRequest.method >> httpRequest.path;

    // Skip headers
    while (std::getline(requestStream, line) && line != "\r") {}

    // Find the start of the body
    const char* bodyStart = strstr(request, "\r\n\r\n");
    if (bodyStart != nullptr) {
        bodyStart += 4; // Skip the "\r\n\r\n"
        httpRequest.bodyLength = length - (bodyStart - request);
        httpRequest.body = new char[httpRequest.bodyLength];
        memcpy(httpRequest.body, bodyStart, httpRequest.bodyLength);
    } else {
        httpRequest.body = nullptr;
        httpRequest.bodyLength = 0;
    }

    return httpRequest;
}

DWORD WINAPI ClientHandler(LPVOID clientSocket) {
    SOCKET client = *(SOCKET*)clientSocket;
    char buffer[4096];
    int bytesRead;
    size_t totalBytesRead = 0;
    char* request = nullptr;

    // Read the request
    while ((bytesRead = recv(client, buffer, sizeof(buffer), 0)) > 0) {
        char* newRequest = new char[totalBytesRead + bytesRead];
        if (request) {
            memcpy(newRequest, request, totalBytesRead);
            delete[] request;
        }
        memcpy(newRequest + totalBytesRead, buffer, bytesRead);
        request = newRequest;
        totalBytesRead += bytesRead;

        // Check if we have received the complete request
        if (strstr(request, "\r\n\r\n") != nullptr) {
            break;
        }
    }

    if (totalBytesRead > 0) {
        // Parse the request
        HttpRequest httpRequest = parseRequest(request, totalBytesRead);

        // Log the request details
        std::cout << "Method: " << httpRequest.method << std::endl;
        std::cout << "Path: " << httpRequest.path << std::endl;
        std::cout << "Body: ";
        for (size_t i = 0; i < httpRequest.bodyLength; ++i) {
            std::cout << httpRequest.body[i];
        }
        std::cout << httpRequest.bodyLength << std::endl;
        std::cout << std::endl;

        // Send the response
        send(client, responseHeader, strlen(responseHeader), 0);
        send(client, responseBody, strlen(responseBody), 0);

        // Clean up
        delete[] httpRequest.body;
    }

    // Close the client socket
    closesocket(client);
    delete[] request;
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
