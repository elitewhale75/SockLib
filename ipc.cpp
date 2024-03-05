#include <iostream>
#include "ipc.h"

int handler::listen(){
    std::cout << "Listening" << std::endl;
    return 0;
}

int handler::send(){
    std::cout << "Sending" << std::endl;
    return 0;
}
