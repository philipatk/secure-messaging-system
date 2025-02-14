#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

class TCPclient {
private:
    std::string m_serverAddress;
    std::string m_port;
    SOCKET m_socket;    
public:
    TCPclient(const std::string& serverAddress, const std::string& port);
    ~TCPclient();

    bool connectToServer();
    bool sendMessage(const std::string& message);
    std::string receiveMessage();
    void closeConnection();


};

#endif // TCPCLIENT_H
