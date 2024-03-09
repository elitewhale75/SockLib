#include <ipclib/ipc.h>
#include <iostream>

int main () {
    int socketFD;
    sockaddr_un sock;
    char * dataBuffer = (char *) malloc(255);

    // Create Master Socket
    socketFD = server::bindSock(&sock, "/tmp/DemoSocket");

    std::cout << "Opening server socket" << std::endl;
    server::serverListen(socketFD, dataBuffer);

    return 0;
}
