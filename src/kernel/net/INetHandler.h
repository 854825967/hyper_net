#ifndef __INETHANDLER_H__
#define __INETHANDLER_H__
#include "util.h"
#include "NetHeader.h"

class INetHandler {
public:
    INetHandler(const s32 fd) : _fd(fd) {}
    virtual ~INetHandler() {}

    virtual void OnIn() = 0;
    virtual void OnOut() = 0;
    virtual void OnError() = 0;

    s32 GetFD() const { return _fd; }

protected:
    s32 _fd;
};

#endif // __INETHANDLER_H__
