#include <iostream>
#include <limits>

#include "../include/P2Pnode.h"

int main()
{
    try
    {
        P2Pnode node1("20024");


        node1.connectToPeer("127.0.0.1" , "20023");
        
        for (const SOCKET *i : node1.get_connections())
        {
            std::cout << *i;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }catch(...){
        std::cout<< "unknown exeption ...";
    }
    

    return 0;
}