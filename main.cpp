#include <iostream>
#include <sstream>
#include "BlockChain.h"
#include "tests.h"


int main() {
    run_tests();

    BlockChain bc;
    std::cout<< "start";

    bc.add_block("jeden");
    bc.add_block("dwa");
    bc.print();


    return 0;
}




// int main() {
//     BlockChain lancuch = BlockChain();
//     const char* wiadomosc = "Test lancucha znakow";
//     lancuch.add(wiadomosc);
//     const char* wiadomosc2 = "Another message";
//     lancuch.add(wiadomosc2);
//
//
//
//     return 0;
// }
