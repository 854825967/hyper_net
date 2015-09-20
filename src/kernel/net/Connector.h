#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include "INetHandler.h"

namespace core {
    class IPacketParser;
    class ISession;
}

class Connector : public INetHandler
{
public:
    static Connector * Create(const s32 fd) {
        return NEW Connector(fd);
    }

    inline void SetParser(core::IPacketParser * parser) { _parser = parser; }
    inline void SetSession(core::ISession * session) { _session = session; }
    inline void SetBufferSize(const s32 sendSize, const s32 recvSize) { _sendSize = sendSize; _recvSize = recvSize; }
    inline void SetIp(const char * ip, const s32 port) {
        SafeSprintf(_ip, sizeof(ip), ip);
        _port = port;
    }

    virtual void OnIn() {}
    virtual void OnOut();
    virtual void OnError();

private:
    Connector(const s32 fd);
    virtual ~Connector();

private:
    core::IPacketParser * _parser;
    core::ISession * _session;

    s32 _sendSize;
    s32 _recvSize;

    char _ip[MAX_IP_SIZE];
    s32 _port;
};

#endif // __CONNECTOR_H__
