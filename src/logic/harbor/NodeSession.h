#ifndef __NODESESSION_H__
#define __NODESESSION_H__
#include "util.h"
#include "IKernel.h"
using namespace core;
#include "Define.h"

class NodeSession : public ISession {
public:
    NodeSession();
    virtual ~NodeSession();

    inline void SetConnect(const char * ip, const s32 port) {
        _connect = true;
		SafeSprintf(_ip, sizeof(_ip), ip);
        _port = port;
    }

    virtual void OnConnected(IKernel * kernel);
    virtual void OnRecv(IKernel * kernel, const void * context, const s32 size);
    virtual void OnError(IKernel * kernel, const s32 error);
    virtual void OnDisconnected(IKernel * kernel);
    virtual void OnConnectFailed(IKernel * kernel);

    bool PrepareSendNodeMessage(const s32 size);
    bool SendNodeMessage(const void * context, const s32 size);

private:
    bool _ready;
    s32 _nodeType;
    s32 _nodeId;

    bool _connect;
    char _ip[MAX_IP_SIZE];
    s32 _port;
};

#endif //__NODESESSION_H__
