#ifndef SOCKLIB
#define SOCKLIB
#include <sys/un.h>
#include <string>

namespace server{
    /**
     * @brief Creates commmunication endpoint from 
     * file descriptor specified in path
     *
     * @param sock Central socket struct
     * @param sockFile Path to file for socket creation
     * @return master file descriptor for server process
     */
    int bind_socket(struct sockaddr_un * sock , std::string sockFile);

    /**
     * @brief Prepare master socket to listen to n number of connections
     *
     * @param socketFD Socket for server process to listen to
     * @param connections number of connections
     * @return 0 upon successful completion
     */
    void server_listen(int masterSocket, int connections);

    /**
     * @brief Check for messages from client process
     *
     * @param Master socket file descriptor to listen to
     * @return New file descriptor for data to pass between proccesses
     */
    int server_accept(int masterSocket);

};

namespace client{
    /**
     * @brief Creates data socket from file path
     *
     * @param sock Data socket struct
     * @param sockFile Path to file for socket creation
     * @return data file descriptor for client process
     */
    int create_socket(struct sockaddr_un * sock , std::string sockFile);
    /**
     * @brief Establish connection to master process for message passing
     *
     * @param sock Data socket struct
     * @param dataSocket File descriptor for message passing
     * @return 0 upon successful completion
     */
    int connect(struct sockaddr_un * sock , int dataSocket);
};

namespace sock{

    /**
     * @brief Send message to master and read response from master
     *
     * @param socketFD File descriptor for data socket
     * @param dataBuffer Memory space to send and read messages from
     * @return 0 upon successful completion and new data to dataBuffer
     */
    int msg_send(int socketFD , std::string message);
    /**
     * @brief
     *
     * @param
     * @return
     */
    std::string msg_receive(int data_socket);
};
#endif
