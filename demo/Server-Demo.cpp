#include <sock/sock.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <poll.h>
#include <map>

#define SOCK_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128
const int MAX_CONNECTIONS = 3;

void pollin_operation(int data_fd);
void pollout_operation(int data_fd, std::string message);

int main () {
    int             ret;
    int             master_socket_fd;
    int             ready;
    int             active_processes;
    sockaddr_un     sock;
    std::string     buffer;
    std::string     users[3];
    struct pollfd * pfds;
    std::map<int, std::string> user_map;

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

        // Monitor for new user connections
        ret = server::monitor_connections (pfds, MAX_CONNECTIONS, &active_processes);
        if (ret == -1) {
            perror("monitor");
        } else if (ret > 0) {
            int user_fd = ret;
            // Add new user to user table
            std::string user_name = sock::msg_receive (ret);
            std::string msg = "Hello " + user_name + "\n";
            sock::msg_send (ret, msg);

            // Keep track of users
            user_map[ret] = user_name;
            for(const auto& pair : user_map){
                std::cout << pair.first << "\t" << pair.second << "\n";
            }
        }

        // Examine all connections
        for ( int i = 1 ; i < MAX_CONNECTIONS ; i++ ){
            // If client disconnects, remove them from poll fds
            if (pfds[i].revents & POLLHUP){ /* POLLERR | POLLHUP */
                int client_fd = pfds[i].fd;
                std::string username = user_map[client_fd];
                std::cout << "Closing process " << client_fd << "\n";
                std::cout << "Goodbye " << username << '\n';
                user_map.erase(client_fd);
                close(pfds[i].fd);
                pfds[i].fd = ~pfds[i].fd;
                active_processes--;

            } else if ( pfds[i].revents & POLLIN ){ // Client has data to process
                pollin_operation(pfds[i].fd);
            } else if (pfds[i].revents & POLLOUT){ // Send Data to Client

            } else {

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
