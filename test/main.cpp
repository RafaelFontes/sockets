#include <iostream>

#include "sock-test.hpp"

int main() {

    try {
        sock_test();
    }
    catch(const char * e)
    {
        std::cout << e << std::endl;
    }

    return 0;
}
