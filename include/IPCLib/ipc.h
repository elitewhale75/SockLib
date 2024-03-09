#include <sys/un.h>

namespace server {
    int bindSocket(struct sockaddr_un * sock , char const * sockFile);
    int serverListen(int socketFD, int connections);
    int serverAccept(int socketFD);
};

namespace client{
    int createSocket(struct sockaddr_un * sock , char const * sockFile);
    int send(int socketFD , char * dataBuffer);
};
