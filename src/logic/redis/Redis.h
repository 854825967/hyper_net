#ifndef __REDIS_H__
#define __REDIS_H__
#include "util.h"
#include "IModule.h"

class Redis : public IModule {
public:
    virtual bool Initialize(IKernel * kernel);
    virtual bool Launched(IKernel * kernel);
    virtual bool Destroy(IKernel * kernel);
    virtual void Loop(IKernel * kernel) {}

	static Redis * Self() { return s_self; }

private:
	static Redis * s_self;
    IKernel * _kernel;
};

#endif //__REDIS_H__

