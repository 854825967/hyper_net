#ifndef __NETHEADER_H__
#define __NETHEADER_H__
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>

inline bool SetNonBlock(const s32 fd) {
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK) == 0;
}

inline bool SetHardClose(const s32 fd) {
    linger info;
    info.l_linger = 0;
    info.l_onoff = 1;
    return setsockopt(fd, SOL_SOCKET, SO_LINGER, (const char*)&info, sizeof(info)) == 0;
}

inline bool SetNonNegal(const s32 fd) {
    long val = 1l;
    return setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&val, sizeof(val)) == 0;
}

namespace net {
    const s32 INVALID_INDEX = -1;
    const s32 INVALID_FD = -1;
}

#define MAX_IP_SIZE 32

#endif // __NETHEADER_H__
