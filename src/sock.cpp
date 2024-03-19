#include "../include/SockLib/sock.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>

#define BUFFER_SIZE 128

//TODO Replace character pointers to strings
int server::bindSocket(struct sockaddr_un * sock , std::string sockFile){

    int ret;
    int connection_socket;

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

    /*Initialize socket*/
    memset(sock, 0, sizeof(struct sockaddr_un));

    /*Specify socket credentials*/
    sock -> sun_family = AF_UNIX;
    strncpy(sock->sun_path, sockFile.c_str(), sizeof(sock->sun_path) - 1);

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

int server::serverListen(int masterSocket, int maxConnections){
    int ret;

    ret = listen(masterSocket, maxConnections);
    if(ret == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int server::serverAccept(int masterSocket){
    int dataSocket; // Carries out actual data exchange with client
    /* Accept syscall and initialize data file descriptor */
    dataSocket = accept(masterSocket, NULL, NULL);
    if(dataSocket == -1){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "Process connection established" << '\n';

    return dataSocket;
}

int client::createSocket(struct sockaddr_un * sock , std::string sockFile){
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

int client::send(int dataSocket , char * dataBuffer){
    int ret;

    /* Send data to server from buffer */
    ret = write(dataSocket, dataBuffer, BUFFER_SIZE );
    if(ret == -1){
        perror("write");
        exit(EXIT_FAILURE);
    }

    /* Request data from server */
    memset(dataBuffer, 0, BUFFER_SIZE);
    ret = read(dataSocket, dataBuffer, BUFFER_SIZE ); 
    if(ret == -1){
        perror("read");
        exit(EXIT_FAILURE);
    }
    std::cout << "Recvd from server process: " << dataBuffer << '\n';

    return 0;
}
