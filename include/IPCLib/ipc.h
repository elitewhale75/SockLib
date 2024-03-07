#include <sys/un.h>

namespace ipc {
    int ipcListen(int socketFD);
    int send();
    int bindSock(sockaddr_un * sock , char const * sockName);
};
