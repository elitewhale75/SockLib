#include <sock/sock.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <poll.h>

#define SOCK_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128
const int MAX_CONNECTIONS = 3;


int main () {

    int             master_socket_fd;
    int             data_fd;
    sockaddr_un     sock;
    int             ret;
    std::string     buffer;
    
    // Create Master Socket
    master_socket_fd = server::bind_socket(&sock, SOCK_NAME);
    
    // Client FDs for maintaining order. Master FD is also member
    int active_procceses = 0;
    struct pollfd pfds[MAX_CONNECTIONS];
    pfds[active_procceses].fd = master_socket_fd;
    pfds[active_procceses].events = POLLIN;
    active_procceses++;
    

    // Open server process to 3 client proccesses
    server::server_listen(master_socket_fd, MAX_CONNECTIONS);
    while (active_procceses < MAX_CONNECTIONS){
        if ( (data_fd = server::server_accept(master_socket_fd)) > 0 ){
            pfds[active_procceses].fd = data_fd;
            pfds[active_procceses].events = POLLIN | POLLOUT;
            std::cout << "Process " << pfds[active_procceses].fd << " accepted" << '\n';
            active_procceses++;
        }
    }

    while(1){
        int tmp = poll(pfds, MAX_CONNECTIONS, 0);
        for ( int i = 1 ; i < MAX_CONNECTIONS ; i++ ){
            if ( pfds[i].revents & POLLIN){
                buffer = sock::msg_receive(pfds[i].fd);
                for ( int j = 0 ; j < buffer.length() ; j++ )
                    if ( buffer[j] == 'l' )
                        buffer[j] = 'w';
                sock::msg_send(pfds[i].fd, buffer);
            }
        }
    }
    // Perform clean up before terminating server proccess
    close(data_fd);
    close(master_socket_fd);
    return 0;
}
