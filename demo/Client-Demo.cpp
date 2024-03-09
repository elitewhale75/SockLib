#include <ipclib/ipc.h>
#include <iostream>
#include <unistd.h>

#define SOCK_NAME "/tmp/DemoSocket"

int main () {
    int dataFile;
    sockaddr_un sock;
    char * dataBuffer = (char * ) malloc(128);

    dataFile = client::createSocket(&sock, SOCK_NAME);

    for (;;){

        fgets(dataBuffer, 128, stdin);
        client::send(dataFile, dataBuffer);
        close(dataFile);

    }
    return 0;
}

