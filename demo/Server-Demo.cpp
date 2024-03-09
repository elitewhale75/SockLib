#include <ipclib/ipc.h>
#include <iostream>

#define SOCK_NAME "/tmp/DemoSocket"

int main () {
    int socketFD;
    sockaddr_un sock;
    char * dataBuffer = (char *) malloc(255);

    // Create Master Socket
    socketFD = server::bindSock(&sock, SOCK_NAME);

    // Prepare for accepting connections
    server::serverListen(socketFD, 3);

    return 0;
}
