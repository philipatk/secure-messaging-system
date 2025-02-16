#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <list>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

class P2Pnode {
private:
    std::list<SOCKET> m_connections;  // List of active peer connections
    std::string m_listening_port;      // Port the node listens on
    SOCKET m_listen_socket;            // Server socket for incoming connections

public:
    P2Pnode(const std::string& port);
    ~P2Pnode();

    void setup();  // Initializes Winsock and starts listening
    void startListening();
    void stopListening();
    bool sendMessage(SOCKET peer, const std::string& message);
    void acceptConnections();  // Accepts incoming peer connections
    void closeConnections();   // Closes all sockets and cleans up
};
