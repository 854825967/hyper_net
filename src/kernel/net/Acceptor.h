#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "INetHandler.h"

namespace core {
    class IPacketParser;
    class ISessionFactory;
}

class Connection;
class Acceptor : public INetHandler {
public:
    static Acceptor * Create(const s32 fd) {
        return NEW Acceptor(fd);
    }

    inline void SetParser(core::IPacketParser * parser) { _parser = parser; }
    inline void SetFactory(core::ISessionFactory * factory) { _factory = factory; }
    inline void SetBufferSize(const s32 sendSize, const s32 recvSize) { _sendSize = sendSize; _recvSize = recvSize; }

    virtual void OnIn();
    virtual void OnOut() {}
    virtual void OnError();

private:
    Acceptor(const s32 fd);
    virtual ~Acceptor();

    Connection * accept();

private:
    core::IPacketParser * _parser;
    core::ISessionFactory * _factory;

    s32 _sendSize;
    s32 _recvSize;
};

#endif // __ACCEPTOR_H__
