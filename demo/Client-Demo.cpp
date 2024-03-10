#include <ipclib/ipc.h>
#include <iostream>
#include <unistd.h>

#define SOCK_NAME "/tmp/DemoSocket"

int main () {
    int dataFile;
    sockaddr_un sock;
    char * dataBuffer = (char * ) malloc(128);

    // Create socket for data proccessing from path
    dataFile = client::createSocket(&sock, SOCK_NAME);

    // Attempt to establish connection with master proccess
    client::connect(&sock, dataFile);

    // Send message to master proccess and receive response
    fgets(dataBuffer, 128, stdin);
    client::send(dataFile, dataBuffer);

    // Close file descriptors after use
    close(dataFile);
    return 0;
}
