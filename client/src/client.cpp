#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdint>
#include <iostream>

SOCKET createClientSocket(const char* ip, unsigned port)
{
    // Initiliaze WSA

    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0)
    {
        std::cout << "The Winsock dll not found!" << std::endl;
        return INVALID_SOCKET;
    }

    // Creating the client socket

    SOCKET clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Connect the socket

    SOCKADDR_IN clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_port = htons(port);
    clientService.sin_addr.s_addr = inet_addr(ip);

    if (connect(clientSocket, (SOCKADDR*) &clientService, sizeof(clientService)) == SOCKET_ERROR)
    {
        std::cout << "Error at client connect(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return INVALID_SOCKET;
    }

    return clientSocket;
}

void receivePath(SOCKET clientSocket)
{
    uint8_t status;

    int bytesReceived = recv(clientSocket, (char*)&status, 1, 0);
    if (bytesReceived <= 0) 
    {
        std::cerr << "Failed to receive response from the server" << std::endl;
        return;
    }

    if (status == 0x00) 
    {
        // Error message was sent
        char buffer[1024];
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) 
        {
            buffer[bytesReceived] = '\0';
            std::cout << "Server error: " << buffer << std::endl;
        } 
        else std::cout << "Failed to receive error message." << std::endl;
        return;

    } else if (status != 0x01) 
    {
        std::cerr << "Unknown status code." << std::endl;
        return;
    }

    // Receiving path size
    uint32_t netSize;
    bytesReceived = recv(clientSocket, (char*)&netSize, sizeof(netSize), 0);
    if (bytesReceived != sizeof(netSize)) 
    {
        std::cerr << "Failed to receive path size." << std::endl;
        return;
    }
    uint32_t pathSize = ntohl(netSize);

    // Receiving each node
    for (uint32_t i = 0; i < pathSize; ++i) 
    {
        uint32_t netNode;
        bytesReceived = recv(clientSocket, (char*)&netNode, sizeof(netNode), 0);
        if (bytesReceived != sizeof(netNode)) 
        {
            std::cerr << "Failed to receive node " << i << std::endl;
            break;
        }
        uint32_t node = ntohl(netNode);
        std::cout << node << " ";
    }

    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    // Creating the client socket
    SOCKET client = createClientSocket("127.0.0.1", 55555);

    // Sending and receiving data to the server
    std::string request;

    //Sending requests
    while (true) 
    {
        std::cout << "> ";
        std::getline(std::cin, request);

        if (request == "exit") break;
        if (request.empty()) continue;

        send(client, request.c_str(), request.size(), 0);
        receivePath(client);
    }

    closesocket(client);
    WSACleanup();
    return 0;
}