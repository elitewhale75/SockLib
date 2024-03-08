#include <iostream>
#include "../include/IPCLib/ipc.h"
#include <unistd.h>
#include <sys/socket.h>

#define DATA_SIZE 255
#define BUFFER_SIZE 128
#define MAX_CONNECTS 3

/* TODO Split listen and accept+read syscalls into different routines*/
int server::serverListen(int socketFD , char * dataBuffer){
    int ret;
    int dataSocket; // Carries out actual data exchange with client
    int result;

    /* Prepare for accepting connections, with backlog size 20 */
    ret = listen(socketFD, MAX_CONNECTS);
    if(ret == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* Accept syscall */
    dataSocket = accept(socketFD, NULL, NULL);
    if(dataSocket == -1){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "Connection accepted from client" << std::endl;

    /* Prepare buffer */
    memset(dataBuffer, 0, BUFFER_SIZE);

    /* Wait for next data packet
     * Block server process. Wait for data to arrive from client */
    std::cout << "Waiting for data from client" << std::endl;
    ret = read(dataSocket, dataBuffer, DATA_SIZE);
    if(ret == -1){
        perror("read");
        exit(EXIT_FAILURE);
    }

    /* Send data back to client */
    printf("%s\n", dataBuffer);

    std::cout << "Sending response back to client" << std::endl;
    ret = write(dataSocket, dataBuffer, BUFFER_SIZE);
    if(ret == -1){
        perror("write");
        exit(EXIT_FAILURE);
    }
    /* Close Socket */
    close(dataSocket);
    /* Close Master Socket */
    close(socketFD);
    /* Server should release resources before getting terminated */
    // unlink(SOCKET_NAME);
    
    /* Server operations executed successfully */
    return 0;
}

int client::send(){
    std::cout << "Sending to process: " << std::endl;
    return 0;
}

//TODO Replace character pointers to strings
int server::bindSock(sockaddr_un * sock , char const * sockFile){

    int ret;
    int connection_socket;

    /* In case program exiteded inadvertently on last run
     * remove socket */
    unlink(sockFile);
    /* Create Master Socket */
    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if(connection_socket == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket was created successfully" << std::endl;

    /*Initialize socket*/
    memset(sock, 0, sizeof(struct sockaddr_un));

    /*Specify socket credentials*/
    sock -> sun_family = AF_UNIX;
    strncpy(sock->sun_path, sockFile, sizeof(sock->sun_path) - 1);

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
        << std::endl;

    return connection_socket;
}
