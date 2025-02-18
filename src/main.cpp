#include <iostream>
#include <limits>

#include "../include/P2Pnode.h"
int options()
{
    int choice;

    std::cout << "Choose the option that you want:\n"
              << "1. Connect to node\n"
              << "2. Stop listening\n"
              << "3. Start listening\n"
              << "4. Exit\n";

    std::cout << "Enter your choice: ";
    std::cin >> choice;

    if (std::cin.fail())
    {                                                                       // Check for input failure
        std::cin.clear();                                                   // Clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid input
        std::cout << "Error: Invalid input. Please enter a number.\n";
        return -1; // Return an error value
    }

    return choice;
}

int main()
{
    try
    {
        P2Pnode node1("20023");

        node1.setup();
        node1.startListening();
        std::cout << "listening on port 20023\n";
        node1.acceptConnections();
        for (const SOCKET *i : node1.get_connections())
        {
            std::cout << *i;
        }
        node1.stopListening();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }catch(...){
        std::cout<< "unknown exeption ...";
    }
    

    return 0;
}