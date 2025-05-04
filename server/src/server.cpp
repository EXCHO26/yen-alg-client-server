#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"

int main(int argc, char* argv[])
{

    int port = 55555;

    // Initiliaze WSA

    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0)
    {
        std::cout << "The Winsock dll not found!" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "The Winsock dll found!" << std::endl;
        std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
    }

    // Creating the server socket

    SOCKET serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }
    else
    {
        std::cout << "socket() is OK!" << std::endl;
    }

    // Bind the socket

    SOCKADDR_IN service;
    service.sin_family = AF_INET;
    service.sin_port = htons(port);
    service.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(serverSocket, (SOCKADDR*) &service, sizeof(service)) == SOCKET_ERROR)
    {
        std::cout << "Error at bind(): " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else
    {
        std::cout << "bind() is OK!" << std::endl;
    }

    // Set the socket to listening

    if (listen(serverSocket, 1) == SOCKET_ERROR)
    {
        std::cout << "Error at listen() : " << WSAGetLastError() << std::endl;
    }
    else
    {
        std::cout << "listen() is OK, waiting for connection..." << std::endl;
    }

    // Accept potential connection

    SOCKET acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET)
    {
        std::cout << "Error at accept(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    std::cout << "Connection established!" << std::endl;
    system("pause");
    WSACleanup();
}