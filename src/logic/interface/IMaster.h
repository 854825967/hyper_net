#ifndef __IMASTER_H__
#define __IMASTER_H__

#include "IKernel.h"
#include "IModule.h"

class IMaster : public IModule {
public:
    virtual ~IMaster() {}

    virtual s32 GetPort() const = 0;
};

#endif //__IMASTER_H__
