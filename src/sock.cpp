#include "../include/SockLib/sock.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>

#define BUFFER_SIZE 128

//TODO: Change from nonblocking to async?
int server::bind_socket(struct sockaddr_un * sock , std::string sockFile){

    int ret;                // Use for error handling
    int connection_socket;  // Return value for master socket

    /* In case program exiteded inadvertently on last run
     * remove socket */
    unlink(sockFile.c_str());

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
    strncpy(sock->sun_path, sockFile.c_str(), sizeof(sock->sun_path) - 1);

    // Set up nonblocking (replace with AIO?)
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

void server::server_listen(int masterSocket, int maxConnections){
    int ret;

    ret = listen(masterSocket, maxConnections);
    if(ret == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

int server::server_accept(int masterSocket){
    int dataSocket; // Carries out actual data exchange with client

    /* Accept syscall and initialize data file descriptor */
    dataSocket = accept(masterSocket, NULL, NULL);
    if(dataSocket == -1){
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

    return dataSocket;
}

int client::create_socket(struct sockaddr_un * sock , std::string sockFile){
    int dataSocket;

    /* Create data socket */
    dataSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if(dataSocket == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Clear whole structure and populate standard fields */
    memset(sock, 0, sizeof(struct sockaddr_un));

    /* Connect to socket address. Non-blocking syscall */
    sock -> sun_family = AF_UNIX;
    strncpy(sock->sun_path, sockFile.c_str(), sizeof(sock->sun_path) - 1);
    return dataSocket;
}

int client::connect(struct sockaddr_un * sock , int dataSocket){
    int ret;

    ret = connect(dataSocket, 
            (const struct sockaddr *) sock, 
            sizeof(sockaddr_un));
    if(ret ==-1){
        fprintf(stderr, "Server process is dead");
        exit(EXIT_FAILURE);
    }
    return 0;
}

// TODO: Send and recv
int sock::msg_send(int dataSocket , std::string message){
    int ret;

    /* Send data to server from buffer */
    ret = send(dataSocket, message.c_str(), message.size(), MSG_WAITALL);
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
