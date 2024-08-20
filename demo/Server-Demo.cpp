#include <sock/sock.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <poll.h>

#define SOCK_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128
const int MAX_CONNECTIONS = 3;

void pollin_operation(int data_fd);
void pollout_operation(int data_fd, std::string message);

int main () {
    int             master_socket_fd;
    int             ready;
    int             active_processes;
    sockaddr_un     sock;
    std::string     buffer;
    struct pollfd * pfds;

    // Create Master Socket
    master_socket_fd = server::bind_socket(&sock, SOCK_NAME);
    
    //  Create array for server and client connections Master FD is also member
    pfds = server::init_poll(master_socket_fd, MAX_CONNECTIONS);

    std::cout << "Begin monitoring client proccesses" << '\n';
    active_processes = 1;
    while(1){
        // Monitor file descriptors
        ready = poll(pfds, MAX_CONNECTIONS, 0);
        if ( ready == -1 ){
            perror("ready");
            exit(EXIT_FAILURE);
        }

        server::monitor_poll(pfds, MAX_CONNECTIONS, &active_processes);
        // Examine all connections
        for ( int i = 1 ; i < MAX_CONNECTIONS ; i++ ){
            if ( pfds[i].revents & POLLIN ){ // Service Client Request
                pollin_operation(pfds[i].fd);
            } else if (pfds[i].revents & POLLOUT){ // Send Data to Client

            } else { /* POLLERR | POLLHUP */

            }
        }
    }
    // Perform clean up before terminating server proccess
    close(master_socket_fd);
    free(pfds);
    return 0;
}

void pollin_operation(int data_fd){
    std::string         buffer;

    buffer = sock::msg_receive(data_fd);

    // Replace all ls in message to ws and send back to client
    for ( int j = 0 ; j < buffer.length() ; j++ )
        if ( buffer[j] == 'l' )
            buffer[j] = 'w';

    sock::msg_send(data_fd, buffer);
}

void pollout_operation(int data_fd, std::string message){

}
