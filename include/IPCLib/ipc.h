#include <sys/un.h>

namespace server {
    int serverListen(int socketFD , char * dataBuffer);
    int bindSock(sockaddr_un * sock , char const * sockFile);
};

namespace client{
    int send();
};
