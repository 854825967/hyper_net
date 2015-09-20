#ifndef __MASTER_H__
#define __MASTER_H__
#include "util.h"
#include "IMaster.h"
#include "IHarbor.h"
#include <unordered_map>
#include "Define.h"

class Master : public IMaster, public INodeListener {
    struct NodeInfo {
        s32 nodeType;
        s32 nodeId;
        char ip[MAX_IP_SIZE];
        s32 port;
    };

public:
    virtual bool Initialize(IKernel * kernel);
    virtual bool Launched(IKernel * kernel);
    virtual bool Destroy(IKernel * kernel);
    virtual void Loop(IKernel * kernel) {}

    virtual void OnOpen(IKernel * kernel, s32 nodeType, s32 nodeId, bool hide, const char * ip, s32 port);
    virtual void OnClose(IKernel * kernel, s32 nodeType, s32 nodeId);

    virtual s32 GetPort() const { return s_port; }

    static Master * Self() { return s_self; }

private:
    static void SendNewNode(IKernel * kernel, s32 nodeType, s32 nodeId, s32 newNodeType, s32 newNodeId, const char * ip, s32 port);

private:
    static Master * s_self;
    IKernel * _kernel;
    static IHarbor * s_harbor;

    static s32 s_port;
    static std::unordered_map<s64, NodeInfo> s_nodes;
};

#endif //__MASTER_H__

