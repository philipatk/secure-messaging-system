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

    const std::list<const SOCKET *> get_connections() const;
    std::string get_listening_port() const;
    SOCKET get_listening_socket() const;
    void setup(); // Initializes sucket and binds
    void startListening();
    void stopListening();
    bool sendMessage(SOCKET peer, const std::string &message);
    void acceptConnections();  // Accepts incoming peer connections
    void connectToPeer(const std::string &ip, const std::string &port);
    void closeConnections(); // Closes all sockets
};
