#include <iostream>
#include "../include/P2Pnode.h"
int main (){
    try
    {
        P2Pnode node1("20023");

        node1.setup();
        node1.startListening();
        node1.stopListening();
        node1.startListening();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }catch(...){
        std::cout<< "unknown exeption ...";
    }
    

    return 0;
}