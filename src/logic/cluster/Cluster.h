#ifndef __CLUSTER_H__
#define __CLUSTER_H__
#include "util.h"
#include "IModule.h"
#include <unordered_set>

class IHarbor;
class Cluster : public IModule {
public:
    virtual bool Initialize(IKernel * kernel);
    virtual bool Launched(IKernel * kernel);
    virtual bool Destroy(IKernel * kernel);
    virtual void Loop(IKernel * kernel) {}

    static Cluster * Self() { return s_self; }

private:
    static void NewNodeComming(IKernel * kernel, s32 nodeType, s32 nodeId, const void * context, const s32 size);

private:
    static Cluster * s_self;
    IKernel * _kernel;
    static IHarbor * s_harbor;

    static std::unordered_set<s64> s_openNode;
    static std::string _ip;
    static s32 _port;
};

#endif //__CLUSTER_H__

