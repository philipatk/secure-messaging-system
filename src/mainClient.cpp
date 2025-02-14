#include "../include/TCPclient.h"
#include <iostream>

int main() {
    std::string serverAddress;
    std::cout << "Enter server address: ";
    std::cin >> serverAddress;
    std::cin.ignore(); // Ignore leftover newline character

    TCPclient client(serverAddress, "27015");

    if (!client.connectToServer()) {
        return 1;
    }

    std::string message;
    while (true) {
        std::cout << "Enter message (or 'exit' to quit): ";
        std::getline(std::cin, message);

        if (message == "exit") {
            break;
        }

        if (!client.sendMessage(message)) {
            std::cerr << "Failed to send message. Exiting..." << std::endl;
            break;
        }

        std::string response = client.receiveMessage();
        if (response.empty()) {
            std::cerr << "Server disconnected or an error occurred." << std::endl;
            break;
        }

        std::cout << "Server response: " << response << std::endl;
    }

    client.closeConnection();
    return 0;
}
