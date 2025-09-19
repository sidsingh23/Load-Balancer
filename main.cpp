#include <iostream>
#include <winsock2.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (wsaResult != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }
    
    // Create socket
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed" << endl;
        WSACleanup();
        return 1;
    }
    
    // Bind to port 8080
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    
    int bindResult = bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (bindResult == SOCKET_ERROR) {
        cerr << "Bind failed" << endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    
    // Listen for connections
    int listenResult = listen(listenSocket, SOMAXCONN);
    if (listenResult == SOCKET_ERROR) {
        cerr << "Listen failed" << endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    
    cout << "Load balancer listening on port 8080..." << endl;
    
    // Accept one connection for now
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Accept failed" << endl;
    } else {
        cout << "Client connected!" << endl;
        closesocket(clientSocket);
    }
    
    closesocket(listenSocket);
    WSACleanup();
    return 0;
}