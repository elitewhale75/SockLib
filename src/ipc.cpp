#include <iostream>
#include "../include/IPCLib/ipc.h"
#include <unistd.h>
#include <sys/socket.h>

#define BUFFER_SIZE 128
#define MAX_CONNECTS 3

int ipc::ipcListen(int socketFD){
    int ret;
    int data_socket;
    int result;

    /* Prepare for accepting connections, with backlog size 20 */
    ret = listen(socketFD, MAX_CONNECTS);
    if(ret == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* Server process loop for handling connections */
    for(;;){

        /* Accept is blocking syscall */
        data_socket = accept(socketFD, NULL, NULL);
        if(data_socket == -1){
            perror("accept");
            exit(EXIT_FAILURE);
        }
        std::cout << "Connection accepted from client" << std::endl;

        for(;;){

        }
    }
    return -1;
}

int ipc::send(){
    std::cout << "Sending to process: " << std::endl;
    return 0;
}

//TODO Replace character pointers to strings
int ipc::bindSock(sockaddr_un * sock , char const * sockName){

    int ret;
    int connection_socket;

    /* In case program exiteded inadvertently on last run
     * remove socket */
    unlink(sockName);
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
    strncpy(sock->sun_path, sockName, sizeof(sock->sun_path) - 1);

    /*Bind syscall*/
    ret = bind(connection_socket, 
            (const struct sockaddr *) sock, 
            sizeof(sockaddr_un));
    if(ret == -1){
        perror("bind");
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket successfully binded" << std::endl;

    return connection_socket;
}
