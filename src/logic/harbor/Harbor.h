#ifndef __HARBOR_H__
#define __HARBOR_H__
#include "util.h"
#include "IHarbor.h"
#include <list>
#include <unordered_map>
#include "Define.h"

#pragma pack(push, 1)
struct HarborHeader {
    s32 message;
    s32 len;
};

struct NodeReport {
    s32 nodeType;
    s32 nodeId;
    s32 port;
    bool hide;
};
#pragma pack(pop)

namespace harbor {
    enum MessageID {
        REPORT,
        MESSAGE,
    };
}

struct NodeListenerUnit {
    INodeListener * listener;
    char debug[MAX_DEBUG_SIZE];
};

struct Handler {
    NodeProtocolHandlerType func;
    char debug[MAX_DEBUG_SIZE];
};

class NodeSession;
class Harbor : public IHarbor, public IPacketParser, public ISessionFactory {
    struct Reconnect {
        NodeSession * session;
        char ip[MAX_IP_SIZE];
        s32 port;
        s64 tick;
    };

public:
    virtual bool Initialize(IKernel * kernel);
    virtual bool Launched(IKernel * kernel);
    virtual bool Destroy(IKernel * kernel);
    virtual void Loop(IKernel * kernel);

    virtual s32 ParsePacket(const void * context, const s32 size);

    virtual ISession * Create();
    virtual void Recover(ISession *);

    virtual void Connect(const char * ip, const s32 port);
    virtual void AddNodeListener(INodeListener * listener, const char * debug);

    virtual bool PrepareSend(s32 nodeType, s32 nodeId, const s32 messageId, const s32 size);
    virtual bool Send(s32 nodeType, s32 nodeId, const void * context, const s32 size);

    virtual void PrepareBrocast(s32 nodeType, const s32 messageId, const s32 size);
    virtual void Brocast(s32 nodeType, const void * context, const s32 size);

    virtual void RegProtocolHandler(s32 nodeType, s32 messageId, const NodeProtocolHandlerType& handler, const char * debug);

    virtual s32 GetNodeType() const { return _nodeType; }
    virtual s32 GetNodeId() const { return _nodeId; }
    s32 GetPort() const { return _port; }
    bool IsHide() const { return _hide; }

    void OnNodeOpen(IKernel * kernel, const s32 nodeType, const s32 nodeId, const char * ip, const s32 port, const bool hide, NodeSession * session);
    void OnNodeClose(IKernel * kernel, const s32 nodeType, const s32 nodeId);
    void OnNodeMessage(IKernel * kernel, const s32 nodeType, const s32 nodeId, const void * context, const s32 size);

    void AddReconnect(NodeSession * session, const char * ip, const s32 port);

    static Harbor * Self() { return s_harbor; }

private:
    static Harbor * s_harbor;
    IKernel * _kernel;

    s32 _sendBuffSize;
    s32 _recvBuffSize;
    s32 _reconnectTick;
    s32 _nodeType;
    s32 _nodeId;
    s32 _port;
    bool _hide;

    std::unordered_map<s32, std::unordered_map<s32, NodeSession*>> _nodes;
    std::unordered_map<s32, std::string> _nodeNames;

    std::list<NodeListenerUnit> _listeners;
    std::unordered_map<s32, std::unordered_map<s32, Handler>> _handlers;

    std::list<Reconnect*> _reconnects;
};

#endif //__HARBOR_H__

