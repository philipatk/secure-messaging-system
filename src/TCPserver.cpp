#include "../include/TCPserver.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

// Link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")


TCPserver::TCPserver(const std::string& port) 
    : m_port(port), m_listenSocket(INVALID_SOCKET) 
{
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        throw std::runtime_error("WSAStartup failed with error: " + std::to_string(iResult));
    }
}

TCPserver::~TCPserver() {
    if (m_listenSocket != INVALID_SOCKET) {
        closesocket(m_listenSocket);
    }
    WSACleanup();
}

// Set up the server: resolve the address, create socket, bind, and listen.
void TCPserver::setup() {
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;  // Use local IP

    struct addrinfo* result = nullptr;
    int iResult = getaddrinfo(NULL, m_port.c_str(), &hints, &result);
    if (iResult != 0) {
        throw std::runtime_error("getaddrinfo failed with error: " + std::to_string(iResult));
    }

    // Create a SOCKET for the server to listen for client connections.
    m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_listenSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        throw std::runtime_error("Error at socket(): " + std::to_string(WSAGetLastError()));
    }

    // Bind the socket.
    iResult = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);
    if (iResult == SOCKET_ERROR) {
        closesocket(m_listenSocket);
        m_listenSocket = INVALID_SOCKET;
        throw std::runtime_error("bind failed with error: " + std::to_string(WSAGetLastError()));
    }

    // Start listening.
    iResult = listen(m_listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        closesocket(m_listenSocket);
        m_listenSocket = INVALID_SOCKET;
        throw std::runtime_error("listen failed with error: " + std::to_string(WSAGetLastError()));
    }
}

// Accept a client connection; returns the connected client socket.
SOCKET TCPserver::acceptClient() {
    SOCKET clientSocket = accept(m_listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        throw std::runtime_error("accept failed with error: " + std::to_string(WSAGetLastError()));
    }
    return clientSocket;
}

// A simple method to run the echo server for one client.
void TCPserver::run() {
    SOCKET clientSocket = acceptClient();
    std::cout << "Client connected." << std::endl;

    const int bufLen = 512;
    char recvbuf[bufLen];
    int iResult;

    // Echo loop: receive data and send it back.
    do {
        iResult = recv(clientSocket, recvbuf, bufLen, 0);
        if (iResult > 0) {
            // Ensure null termination if the data is text.
            if (iResult < bufLen)
                recvbuf[iResult] = '\0';
            else
                recvbuf[bufLen - 1] = '\0';

            std::cout << "Received: " << recvbuf << " (" << iResult << " bytes)" << std::endl;

            int iSendResult = send(clientSocket, recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                std::cerr << "send failed with error: " << WSAGetLastError() << std::endl;
                break;
            }
            std::cout << "Echoed back " << iSendResult << " bytes." << std::endl;
        }
        else if (iResult == 0) {
            std::cout << "Connection closing..." << std::endl;
        }
        else {
            std::cerr << "recv failed with error: " << WSAGetLastError() << std::endl;
            break;
        }
    } while (iResult > 0);

    // Shutdown the connection.
    shutdown(clientSocket, SD_SEND);
    closesocket(clientSocket);
}
