#include "../include/TCPclient.h"
#include <iostream>
#include <stdexcept>

TCPclient::TCPclient(const std::string& serverAddress, const std::string& port)
    : m_serverAddress(serverAddress), m_port(port), m_socket(INVALID_SOCKET) {
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        throw std::runtime_error("WSAStartup failed with error: " + std::to_string(iResult));
    }
}

TCPclient::~TCPclient() {
    closeConnection();
    WSACleanup();
}

bool TCPclient::connectToServer() {
    struct addrinfo hints = {0}, *result = nullptr, *ptr = nullptr;
    
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int iResult = getaddrinfo(m_serverAddress.c_str(), m_port.c_str(), &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;
        return false;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (m_socket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
            freeaddrinfo(result);
            return false;
        }

        iResult = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(m_socket);
            m_socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (m_socket == INVALID_SOCKET) {
        std::cerr << "Unable to connect to server!" << std::endl;
        return false;
    }

    std::cout << "Connected to server." << std::endl;
    return true;
}

bool TCPclient::sendMessage(const std::string& message) {
    int iResult = send(m_socket, message.c_str(), (int)message.length(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "Send failed with error: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

std::string TCPclient::receiveMessage() {
    const int bufLen = 512;
    char recvbuf[bufLen];
    int iResult = recv(m_socket, recvbuf, bufLen, 0);

    if (iResult > 0) {
        recvbuf[iResult] = '\0';  // Null-terminate the received data
        return std::string(recvbuf);
    } else if (iResult == 0) {
        std::cout << "Connection closed by server." << std::endl;
    } else {
        std::cerr << "Receive failed with error: " << WSAGetLastError() << std::endl;
    }
    return "";
}

void TCPclient::closeConnection() {
    if (m_socket != INVALID_SOCKET) {
        shutdown(m_socket, SD_SEND);
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
}
