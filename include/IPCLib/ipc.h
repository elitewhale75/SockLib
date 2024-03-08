#include <sys/un.h>

namespace server {
    int serverListen(int socketFD , char * dataBuffer);
    int bindSock(struct sockaddr_un * sock , char const * sockFile);
};

namespace client{
    int createSocket(struct sockaddr_un * sock , char const * sockFile);
    int send(int socketFD , char * dataBuffer);
};
