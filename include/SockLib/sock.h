#ifndef SOCKLIB
#define SOCKLIB
#include <string>
#include <sys/un.h>
#include <poll.h>

namespace server{
    /**
     * @brief Creates commmunication endpoint from 
     * file specified in path
     *
     * @param sock Central socket struct
     * @param sockFile Path to file for socket creation
     * @return master file descriptor for server process
     */
    int bind_socket(struct sockaddr_un * sock , std::string sock_file);

    /**
     * @brief Prepare master socket to listen to n number of connections
     *
     * @param socketFD Socket for server process to listen to
     * @param connections number of connections
     * @return 0 upon successful completion
     */
    void server_listen(int master_socket_fd, int connections);

    /**
     * @brief Check for messages from client process
     *
     * @param Master socket file descriptor to listen to
     * @return New file descriptor for data to pass between proccesses
     */
    int server_accept(int master_socket_fd);

    /**
     * @brief Creates an array of poll fds to monitor
     *
     * @param master_socket_fd server file descriptor
     * @param max_connections Number of connections to maintain
     * @return Array of poll fds
     */
    struct pollfd * init_poll (int master_socket_fd, int max_connections);

    /**
     * @brief Checks if there's any pending connections to add to poll array
     *
     * @param pfds Array of poll fds
     * @param max_connections maximum connections to check for
     * @param Number of currently active active_processes
     * @return position of new process in pfds, or 0 for no pending connections
     */
    int monitor_connections (struct pollfd * pfds, int max_connections, 
                             int active_processes);
};

namespace client{
    /**
     * @brief Creates data socket from file path
     *
     * @param sock Data socket struct
     * @param sockFile Path to file for socket creation
     * @return data file descriptor for client process
     */
    int create_socket(struct sockaddr_un * sock , std::string sock_file);
    /**
     * @brief Establish connection to master process for message passing
     *
     * @param sock Data socket struct
     * @param dataSocket File descriptor for message passing
     * @return 0 upon successful completion
     */
    int connect(struct sockaddr_un * sock , int socket_fd);
};

namespace sock{

    /**
     * @brief Send message to a process as a string
     *
     * @param socketFD File descriptor for data socket
     * @param dataBuffer Memory space to send and read messages from
     * @return 0 upon successful completion and new data to dataBuffer
     */
    int msg_send(int socket_fd , std::string message);
    /**
     * @brief Receive message from a process as a string
     *
     * @param data_socket File descriptor for data socket
     * @return Message from client/server process from specified Fd
     */
    std::string msg_receive(int data_socket);
};
#endif
