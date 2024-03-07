#include <sys/un.h>

namespace ipc {
    int listen();
    int send();
    int spawnSocket(sockaddr_un * sock , char const * sockName);
};
