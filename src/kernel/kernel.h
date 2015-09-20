#ifndef __kernel_h__
#define __kernel_h__
#include "util.h"
#include "singleton.h"
#include "IKernel.h"
using namespace core;

class Kernel : public OSingleton<Kernel>, public IKernel {
    friend class OSingleton<Kernel>;
public:

    bool Ready();
    bool Initialize(int argc, char ** argv);
    void Loop();
    void Destroy();

    virtual bool Listen(const char * ip, const s32 port, const s32 sendSize, const s32 recvSize, IPacketParser * parser, ISessionFactory * factory);
    virtual bool Connect(const char * ip, const s32 port, const s32 sendSize, const s32 recvSize, IPacketParser * parser, ISession * session);

    virtual IModule * FindModule(const char * name);
    virtual const char * GetCmdArg(const char * key);

    void Terminate() { _terminate = true; }

private:
    Kernel() : _terminate(false) {}
    virtual ~Kernel() {}

private:
    bool _terminate;
};

#endif //__kernel_h__
