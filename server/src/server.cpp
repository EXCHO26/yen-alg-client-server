#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <string>
#include <cstdint>

#include "Graph.hpp"
#include "ThreadPool.hpp"

void sendStatus(SOCKET acceptSocket, uint8_t status)
{
    send(acceptSocket, (char*)&status, 1, 0);
}

void handleClient(SOCKET acceptSocket) 
{
    char buffer[1024];
    int bytesReceived;

    char filename[256];
    int start, end, kth;

    while ((bytesReceived = recv(acceptSocket, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytesReceived] = '\0';
        std::stringstream stream(buffer);

        if (stream >> filename >> start >> end >> kth)
        {
            try
            {
                std::cout << "Received: " << filename << " " << start << " " << end << " " << kth << std::endl;
            
                Graph myGraph(filename);
                std::vector<Path> allPaths = myGraph.yenKSP(start, end, kth);
                
                if (allPaths.empty()) 
                {
                    // Send error status
                    sendStatus(acceptSocket, 0x00);

                    const char *errorMsg = "No paths found! first";
                    send(acceptSocket, errorMsg , strlen(errorMsg), 0);
                    continue;
                }
    
                size_t sz = std::min((size_t)kth, allPaths.size() - 1);
                const Path& result = allPaths[sz];

                if (result.empty())
                {
                    // Send error status
                    sendStatus(acceptSocket, 0x00);

                    const char *errorMsg = "No paths found!";
                    send(acceptSocket, errorMsg, strlen(errorMsg), 0);
                    continue;
                }

                // Sending status that everything is ok so far
                sendStatus(acceptSocket, 0x01);

                // Sending the size of the result first
                uint32_t resultSize = (uint32_t)result.size();
                uint32_t netSize = htonl(resultSize);
                if (send(acceptSocket, (char*)&netSize, sizeof(netSize), 0) != sizeof(netSize)) 
                {
                    std::cerr << "Failed to send result size.\n";
                    continue;
                }
    
                // Sending the result, node by node
                for (unsigned int node : result) 
                {
                    uint32_t netNode = htonl(node);
                    if (send(acceptSocket, (char*)&netNode, sizeof(netNode), 0) != sizeof(netNode)) 
                    {
                        std::cerr << "Failed to send node.\n";
                        break;
                    }
                }
            }
            catch(const std::exception& e)
            {
                // Send error status
                sendStatus(acceptSocket, 0x00);

                const char *errorMsg = "Error while evaluating the path!";
                send(acceptSocket, errorMsg, strlen(errorMsg), 0) == strlen(errorMsg);
                continue;
            }
        }
        else
        {
            // Send error status
            sendStatus(acceptSocket, 0x00);

            const char *errorMsg = "Something went wrong. Please try again!";
            std::cerr << "Failed to parse input.\n";
            send(acceptSocket, errorMsg, strlen(errorMsg), 0);
        }
    }

    std::cout << "Client disconnected.\n";
    closesocket(acceptSocket);
}

SOCKET createServer(const char* ip, unsigned port)
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

    // Creating the server socket
    SOCKET serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Bind the socket
    SOCKADDR_IN service;
    service.sin_family = AF_INET;
    service.sin_port = htons(port);
    service.sin_addr.s_addr = inet_addr(ip);

    if (bind(serverSocket, (SOCKADDR*) &service, sizeof(service)) == SOCKET_ERROR)
    {
        std::cout << "Error at bind(): " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Set the socket to listening
    if (listen(serverSocket, 1) == SOCKET_ERROR)
    {
        std::cout << "Error at listen() : " << WSAGetLastError() << std::endl;
        return INVALID_SOCKET;
    }

    std::cout << "Server is waiting for connection..." << std::endl;

    return serverSocket;
}

int main(int argc, char* argv[])
{
    ThreadPool pool(4);
    SOCKET server = createServer("127.0.0.1", 55555);

    // Accept potential connection
    while (true)
    {
        sockaddr_in acceptAddr;
        int acceptSize = sizeof(acceptAddr);
        SOCKET acceptSocket = accept(server, (sockaddr*)&acceptAddr, &acceptSize);

        std::cout << "Client connected." << std::endl;
        // Enqueue the client handling task
        pool.enqueue(handleClient, acceptSocket);
    }

    closesocket(server);
    WSACleanup();
    return 0;
}