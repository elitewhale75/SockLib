#include <sock/sock.h>
#include <iostream>
#include <unistd.h>
#include <string>

#define SOCK_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128
const int MAX_CONNECTIONS = 4;


int main () {

    int             master_socket_fd;
    int             data_fd;
    sockaddr_un     sock;
    int             ret;
    std::string     buffer;
    
    // Client FDs for maintaining order. Master FD is also member
    fd_set          monitored_FD_set;

    // Create Master Socket
    master_socket_fd = server::bind_socket(&sock, SOCK_NAME);

    // Open server process to 3 client proccesses
    server::server_listen(master_socket_fd, MAX_CONNECTIONS);

    while(1){
        data_fd = server::server_accept(master_socket_fd);
        sleep(3);
        std::cout << "Connection pending" << '\n';

        if (data_fd != -1){
            // Process request received, service request
            buffer = sock::msg_receive(data_fd);

            // Perform deseired operations in server process
            for( int i = 0 ; i < buffer.length() ; i++)
                if(buffer[i] == 'l')
                    buffer[i] = 'w';

            // Pass message back to client process
            sock::msg_send(data_fd, buffer);
            close(data_fd);
        }
    }
    // Perform clean up before terminating server proccess
    close(master_socket_fd);
    return 0;
}
