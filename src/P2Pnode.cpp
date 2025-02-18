#include "../include/P2Pnode.h"
#include <stdexcept>
#include <list>
#include <iostream>

P2Pnode::P2Pnode(const std::string& listening_port): m_listening_port(listening_port), m_listen_socket(INVALID_SOCKET){
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        throw std::runtime_error("WSAStartup failed with error: " + std::to_string(iResult));
    }
}

const std::list<const SOCKET *> P2Pnode::get_connections() const
{
    std::list<const SOCKET *> result;
    for (const SOCKET &socket : m_connections)
    {
        result.push_back(&socket);
    }
    return result;
}

std::string P2Pnode::get_listening_port() const
{
    return m_listening_port;
}
SOCKET P2Pnode::get_listening_socket() const
{
    return m_listen_socket;
}

void P2Pnode::setup(){
    struct addrinfo hints = {0};  // Initialize struct to zero
    hints.ai_family = AF_INET;     // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_protocol = IPPROTO_TCP; // TCP protocol

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

void P2Pnode::acceptConnections()
{
    SOCKET ClientSocket = INVALID_SOCKET;

    // Accept a client socket
    ClientSocket = accept(m_listen_socket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET)
    {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(m_listen_socket);
        WSACleanup();
        throw std::runtime_error("Failed to accept Connection: " + std::to_string(WSAGetLastError()));
    }
    m_connections.push_back(ClientSocket);
}

void P2Pnode::connectToPeer(const std::string &ip, const std::string &port)
{
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Allow both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    int iResult = getaddrinfo(ip.c_str(), port.c_str(), &hints, &result);
    if (iResult != 0)
    {
        std::cerr << "getaddrinfo failed: " << iResult << std::endl;
        WSACleanup();
        throw std::runtime_error("Failed to resolve server address and port: " + std::to_string(iResult));
    }

    SOCKET ConnectSocket = INVALID_SOCKET;

    // Attempt to connect to one of the addresses returned by getaddrinfo
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        // Create a socket for connecting to the server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET)
        {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
            continue; // Try the next address
        }

        // Attempt to connect
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue; // Try the next address
        }
        break; // Successful connection
    }

    freeaddrinfo(result); // Free memory allocated by getaddrinfo

    if (ConnectSocket == INVALID_SOCKET)
    {
        std::cerr << "Unable to connect to server!" << std::endl;
        throw std::runtime_error("Failed to connect to server: No valid address found.");
    }

    m_connections.push_back(ConnectSocket);
    std::cout << "Connected successfully!" << std::endl;

    // Now ConnectSocket is a valid, connected socket
    // You should store it in a class variable for future communication
}

void P2Pnode::closeConnections()
{
    for (SOCKET i : m_connections)
    {
        closesocket(i);
    }
}

P2Pnode::~P2Pnode(){
    closeConnections();
    closesocket(m_listen_socket);
    WSACleanup();
}