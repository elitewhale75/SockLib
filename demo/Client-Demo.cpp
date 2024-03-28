#include <sock/sock.h>
#include <iostream>
#include <unistd.h>

#define SOCK_NAME "/tmp/DemoSocket"

int main () {
    int dataFile;
    sockaddr_un sock;
    std::string buffer;

    // Create socket for data proccessing from path
    dataFile = client::create_socket(&sock, SOCK_NAME);

    // Attempt to establish connection with master proccess
    client::connect(&sock, dataFile);

    // Send message to master proccess and receive response
    sock::msg_send (dataFile, "hello world");
    buffer = sock::msg_receive(dataFile);
    std::cout << buffer << '\n';

    // Close file descriptors after use
    close(dataFile);

    return 0;
}
