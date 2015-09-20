#ifndef __IKERNEL_H__
#define __IKERNEL_H__
#include "util.h"

class IModule;
namespace core {
    class IKernel;

    class IPipe {
    public:
        virtual ~IPipe() {}

        virtual void Send(const void * context, const s32 size) = 0;
        virtual void Close() = 0;

        virtual const char * GetLocalIp() const = 0;
        virtual s32 GetLocalPort() const = 0;

        virtual const char * GetRemoteIp() const = 0;
        virtual s32 GetRemotePort() const = 0;
    };

    class ISession;
    class ISessionFactory {
    public:
        virtual ~ISessionFactory() {}

        virtual ISession * Create() = 0;
        virtual void Recover(ISession *) = 0;
    };

    class ISession {
    public:
        virtual ~ISession() {}

        virtual void OnConnected(IKernel * kernel) = 0;
        virtual void OnRecv(IKernel * kernel, const void * context, const s32 size) = 0;
        virtual void OnError(IKernel * kernel, const s32 error) = 0;
        virtual void OnDisconnected(IKernel * kernel) = 0;
        virtual void OnConnectFailed(IKernel * kernel) = 0;

        void OnRelease() {
            if (_factory)
                _factory->Recover(this);
        }

        inline void Send(const void * context, const s32 size) {
            if (_pipe)
                _pipe->Send(context, size);
        }

        inline void Close() {
            if (_pipe)
                _pipe->Close();
        }

        inline const char * GetLocalIp() const { return _pipe ? _pipe->GetLocalIp() : nullptr; }
        inline s32 GetLocalPort() const { return _pipe ? _pipe->GetLocalPort() : 0; }

        inline const char * GetRemoteIp() const { return _pipe ? _pipe->GetRemoteIp() : nullptr; }
        inline s32 GetRemotePort() const { return _pipe ? _pipe->GetRemotePort() : 0; }

        void SetPipe(IPipe * pipe) { _pipe = pipe; }
        void SetFactory(ISessionFactory * factory) { _factory = factory; }

    private:
        IPipe * _pipe;
        ISessionFactory * _factory;
    };

    #define INVALID_PACKET -1
    class IPacketParser {
    public:
        virtual ~IPacketParser() {}

        virtual s32 ParsePacket(const void * context, const s32 size) = 0;
    };

    class IKernel {
    public:
        virtual ~IKernel() {}

        virtual bool Listen(const char * ip, const s32 port, const s32 sendSize, const s32 recvSize, IPacketParser * parser, ISessionFactory * factory) = 0;
        virtual bool Connect(const char * ip, const s32 port, const s32 sendSize, const s32 recvSize, IPacketParser * parser, ISession * session) = 0;

        virtual IModule * FindModule(const char * name) = 0;

        virtual const char * GetCmdArg(const char * key) = 0;
    };
}

#define DBG_INFO(format, a...) { \
    char debug[4096] = {0}; \
    SafeSprintf(debug, sizeof(debug), format, ##a); \
    printf("%s\n", debug); \
}

#endif // __IKERNEL_H__
