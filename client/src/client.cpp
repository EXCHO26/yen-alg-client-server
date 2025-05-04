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

    // Creating the client socket

    SOCKET clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }
    else
    {
        std::cout << "socket() is OK!" << std::endl;
    }

    // Connect the socket

    SOCKADDR_IN clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_port = htons(port);
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (SOCKADDR*) &clientService, sizeof(clientService)) == SOCKET_ERROR)
    {
        std::cout << "Error at client connect(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }
    else
    {
        std::cout << "Client connect() is OK" << std::endl;
        std::cout << "Client: Can start sending and receiving data..." << std::endl;
    }

    system("pause");
    WSACleanup();
}