#include "../include/TCPserver.h"

int main() {
    try {
        TCPserver server("27015");
        server.setup();  // Assuming `setup()` initializes the server

        std::cout << "Server started successfully.\n";

        server.run();  // Assuming `run()` keeps the server running
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return 1;  // Exit with error
    } catch (...) {
        std::cerr << "Unknown exception occurred.\n";
        return 1;
    }

    return 0;
}
