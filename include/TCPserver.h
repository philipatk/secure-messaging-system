#include <iostream>
#include <stdexcept>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

// Link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

class TCPserver {

    private:
    std::string m_port;
    SOCKET m_listenSocket;

public:
    TCPserver(const std::string& port);

    ~TCPserver();

    // Set up the server: resolve the address, create socket, bind, and listen.
    void setup();

    // Accept a client connection; returns the connected client socket.
    SOCKET acceptClient();

    // A simple method to run the echo server for one client.
    void run();

};
