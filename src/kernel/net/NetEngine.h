#ifndef __NETENGINE_H__
#define __NETENGINE_H__
#include "util.h"
#include "singleton.h"
#include <unordered_map>
#include "NetHeader.h"
#include <vector>

class INetHandler;
class NetWorker;
namespace core {
    class IPacketParser;
    class ISessionFactory;
    class ISession;
}

class NetEngine : public OSingleton<NetEngine> {
    friend class OSingleton<NetEngine>;

    typedef std::unordered_map<s32, INetHandler*, std::hash<s32>, std::equal_to<s32>, ALLOCATOR(INetHandler*)> HandlerMapType;
public:
    bool Ready();
    bool Initialize();
    void Loop();
    void Destroy();

    bool Listen(const char * ip, const s32 port, const s32 sendSize, const s32 recvSize, core::IPacketParser * parser, core::ISessionFactory * factory);
    bool Connect(const char * ip, const s32 port, const s32 sendSize, const s32 recvSize, core::IPacketParser * parser, core::ISession * session);

    void Add(INetHandler * handler, s32 events);
    void Del(INetHandler * handler, s32 events);

    void Add(INetHandler * handler);
    void Remove(INetHandler * handler);

	NetWorker * GetWorker();

private:
    NetEngine();
    virtual ~NetEngine();

    inline INetHandler * GetHandler(const s32 fd) {
        auto itr = _handlers.find(fd);
        if (itr != _handlers.end()) {
            return itr->second;
        }
        return nullptr;
    }

    s32 CountHandlers() const { return (s32)_handlers.size(); }

private:
    s32 _epollFd;
	epoll_event * _events;

	std::vector<NetWorker*> _workers;

    HandlerMapType _handlers;
};

#endif // __NETENGINE_H__
