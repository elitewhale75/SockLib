#include <ipclib/ipc.h>
#include <iostream>

int main () {
    int socketFD;
    sockaddr_un sock;
    char * dataBuffer = (char *) malloc(128);

    std::cout << "Prompt > ";
    fgets(dataBuffer, 128, stdin);
    std::cout << "Sending data to server" << std::endl;

    socketFD = client::createSocket(&sock,  "/tmp/DemoSocket");
    client::send(socketFD, dataBuffer);

    return 0;
}

