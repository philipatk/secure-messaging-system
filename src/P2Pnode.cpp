#include "../include/P2Pnode.h"
#include <stdexcept>


P2Pnode::P2Pnode(const std::string& listening_port): m_listening_port(listening_port), m_listen_socket(INVALID_SOCKET){
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        throw std::runtime_error("WSAStartup failed with error: " + std::to_string(iResult));
    }
}

void P2Pnode::setup(){
    struct addrinfo hints = {0};  // Initialize struct to zero
    hints.ai_family = AF_INET;     // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_protocol = IPPROTO_TCP; // TCP protocol
    hints.ai_flags = AI_PASSIVE;  // Use the local machine’s IP

    struct addrinfo* result = nullptr;
    int iResult = getaddrinfo(NULL, m_listening_port.c_str(), &hints, &result);
    if (iResult != 0) {
        throw std::runtime_error("getaddrinfo failed with error: " + std::to_string(iResult));
  
    }


    //creates the sucket
    m_listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_listen_socket == INVALID_SOCKET) {
        freeaddrinfo(result);
        throw std::runtime_error("Error at socket(): " + std::to_string(WSAGetLastError()));

    }

    //binds the sucket
    iResult = bind(m_listen_socket, result->ai_addr, (int)result->ai_addrlen);
    freeaddrinfo(result);
    if (iResult == SOCKET_ERROR) {
        closesocket(m_listen_socket);
        m_listen_socket = INVALID_SOCKET;
        throw std::runtime_error("bind failed with error: " + std::to_string(WSAGetLastError()));

    }


}

void P2Pnode::startListening(){
    if ( listen( m_listen_socket, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %ld\n", WSAGetLastError() );
        closesocket(m_listen_socket);
        WSACleanup();
        throw std::runtime_error("failed to start listening to sucket: " + std::to_string(WSAGetLastError()));
    }
}

void P2Pnode::stopListening() {
    if (m_listen_socket == INVALID_SOCKET) {
        throw std::runtime_error("Socket is already closed or invalid.");
    }

    // Set backlog to 0 to stop accepting new connections
    int iResult = listen(m_listen_socket, 0);
    if (iResult == SOCKET_ERROR) {
        throw std::runtime_error("Failed to stop listening: " + std::to_string(WSAGetLastError()));
    }
}


P2Pnode::~P2Pnode(){
    closesocket(m_listen_socket);
    WSACleanup();
}