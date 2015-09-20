#ifndef __STARTER_H__
#define __STARTER_H__
#include "util.h"
#include "IModule.h"

class Starter : public IModule {
public:
    virtual bool Initialize(IKernel * kernel);
    virtual bool Launched(IKernel * kernel);
    virtual bool Destroy(IKernel * kernel);
    virtual void Loop(IKernel * kernel) {}

    static Starter * Self() { return s_self; }

private:
    static Starter * s_self;
    IKernel * _kernel;
};

#endif //__STARTER_H__

