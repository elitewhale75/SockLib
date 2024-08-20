#include "../include/SockLib/sock.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>

#define BUFFER_SIZE 128

int server::bind_socket(struct sockaddr_un * sock , std::string sock_file){

    int ret;                // Use for error handling
    int connection_socket;  // Return value for master socket

    /* In case program exiteded inadvertently on last run
     * remove socket */
    unlink(sock_file.c_str());

    /* Create Master Socket */
    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if(connection_socket == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket was created successfully" << '\n';

    /*Initialize socket credentials*/
    memset(sock, 0, sizeof(struct sockaddr_un));
    sock -> sun_family = AF_UNIX;
    strncpy(sock->sun_path, sock_file.c_str(), sizeof(sock->sun_path) - 1);

    // Set to nonblocking
    fcntl(connection_socket, F_SETFL, O_NONBLOCK);

    /*Bind syscall*/
    ret = bind(connection_socket, 
            (const struct sockaddr *) sock, 
            sizeof(sockaddr_un));
    if(ret == -1){
        perror("bind");
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket successfully binded with status: " 
        << ret 
        << '\n';

    /* Return master file descriptor */
    return connection_socket;
}

void server::server_listen(int master_socket_fd, int max_connections){
    int ret;

    ret = listen(master_socket_fd, max_connections);
    if(ret == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

int server::server_accept(int master_socket_fd){
    int data_socket; // Carries out actual data exchange with client

    /* Accept syscall and initialize data file descriptor */
    data_socket = accept(master_socket_fd, NULL, NULL);
    if(data_socket == -1){
        // No pending connections
        if(errno == EAGAIN || errno == EWOULDBLOCK){
            ;
        }else{
            perror("accept");
            exit(EXIT_FAILURE);
        }
    }else{
        std::cout << "Process connection established" << '\n';
    }

    return data_socket;
}

struct pollfd * server::init_poll (int master_socket_fd, int max_connections){
    struct pollfd *     pfds;
    int                 data_fd;
    
    //TODO handle error with calloc
    pfds = (pollfd *) calloc ( max_connections, sizeof(struct pollfd) );
    pfds[0].fd = master_socket_fd;
    pfds[0].events = POLLIN | POLLOUT;

    server::server_listen (master_socket_fd, max_connections);
    int i = 1;
    for ( ; i < max_connections ; i++){
        pfds[i].fd = -1;
    }
    /*
    while (active_processes < max_connections){
        if ( (data_fd = server::server_accept(master_socket_fd)) > 0 ){
            pfds[active_processes].fd = data_fd;
            pfds[active_processes].events = POLLIN | POLLOUT;
            active_processes++;
        }
    }
    */
    return pfds;
}

int server::monitor_poll (struct pollfd * pfds , int max_connections, int * active_processes){
    int ret;
    int master_socket_fd;
    int current_process_fd;

    if ((*active_processes) >= max_connections) {
        printf("Server process at maximum occupancy\n");
        return 0;
    }
    
    if(!pfds){
        perror("pfds");
        return -1;
    }

    master_socket_fd = pfds[0].fd;
    ret = server::server_accept(master_socket_fd);
    if (ret > 0){
        printf("Searching for available poll slot\n");
        int i = 1;
        for ( ; i < max_connections ; i++){
            current_process_fd = pfds[i].fd;
            if (current_process_fd < 0) {
                pfds[i].fd = ret;
                pfds[i].events = POLLIN | POLLOUT;
                return 0;
            }
        }
    }
    return 0;
}

int client::create_socket(struct sockaddr_un * sock , std::string sock_file){
    int data_socket;

    /* Create data socket */
    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if(data_socket == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Clear whole structure and populate standard fields */
    memset(sock, 0, sizeof(struct sockaddr_un));

    /* Connect to socket address. Non-blocking syscall */
    sock -> sun_family = AF_UNIX;
    strncpy(sock->sun_path, sock_file.c_str(), sizeof(sock->sun_path) - 1);
    return data_socket;
}

int client::connect(struct sockaddr_un * sock , int data_socket){
    int ret;

    ret = connect(data_socket, 
            (const struct sockaddr *) sock, 
            sizeof(sockaddr_un));
    if(ret ==-1){
        fprintf(stderr, "Server process is dead");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int sock::msg_send(int data_socket , std::string message){
    int ret;

    /* Send data to server from buffer */
    ret = send(data_socket, message.c_str(), message.size(), MSG_WAITALL);
    if(ret == -1){
        perror("send");
        exit(EXIT_FAILURE);
    }
    return 0;
}

std::string sock::msg_receive(int data_socket){
    int ret;
    char * data_buffer;
    /* Request data from server */
    data_buffer = (char *) malloc(BUFFER_SIZE);
    memset(data_buffer, 0, BUFFER_SIZE);
    ret = recv(data_socket, data_buffer, BUFFER_SIZE, 0); 
    if(ret == -1){
        perror("recv");
        exit(EXIT_FAILURE);
    }

    std::string message(data_buffer);
    free(data_buffer);
    return message;
}
