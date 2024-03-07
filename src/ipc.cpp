#include <iostream>
#include "../include/IPCLib/ipc.h"
#include <unistd.h>
#include <sys/socket.h>

#define BUFFER_SIZE 128

int ipc::listen(){
    std::cout << "Listening to process: " << std::endl;
    return 0;
}

int ipc::send(){
    std::cout << "Sending to process: " << std::endl;
    return 0;
}

/*Takes in a path a pointer to a socket. Binds socket to address
 * And returns file descriptor */
int ipc::spawnSocket(sockaddr_un * sock , char const * sockName){

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
