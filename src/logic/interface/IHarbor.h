#ifndef __IHARBOR_H__
#define __IHARBOR_H__

#include "IKernel.h"
#include "IModule.h"

class INodeListener {
public:
    virtual ~INodeListener() {}

    virtual void OnOpen(IKernel * kernel, s32 nodeType, s32 nodeId, bool hide, const char * ip, s32 port) = 0;
    virtual void OnClose(IKernel * kernel, s32 nodeType, s32 nodeId) = 0;
};

typedef void (* NodeProtocolHandlerType)(IKernel * kernel, s32 nodeType, s32 nodeId, const void * context, const s32 size);
class IHarbor : public IModule {
public:
    virtual ~IHarbor() {}

    virtual void Connect(const char * ip, const s32 port) = 0;
    virtual void AddNodeListener(INodeListener * listener, const char * debug) = 0;

    virtual bool PrepareSend(s32 nodeType, s32 nodeId, const s32 messageId, const s32 size) = 0;
    virtual bool Send(s32 nodeType, s32 nodeId, const void * context, const s32 size) = 0;

    virtual void PrepareBrocast(s32 nodeType, const s32 messageId, const s32 size) = 0;
    virtual void Brocast(s32 nodeType, const void * context, const s32 size) = 0;

#define ANY_NODE -1
    virtual void RegProtocolHandler(s32 nodeType, s32 messageId, const NodeProtocolHandlerType& handler, const char * debug) = 0;

    virtual s32 GetNodeType() const = 0;
    virtual s32 GetNodeId() const = 0;
};

#define REGPROTOCOL(nodeType, messageId, handler) s_harbor->RegProtocolHandler(nodeType, messageId, handler, #handler)

#endif //__IHARBOR_H__
