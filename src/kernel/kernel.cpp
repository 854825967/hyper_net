#include "kernel.h"
#include "NetEngine.h"
#include "ConfigMgr.h"
#include "LogicMgr.h"
#include "tools.h"

bool Kernel::Ready() {
    if (ConfigMgr::Instance() == nullptr)
        return false;
    if (NetEngine::Instance() == nullptr)
        return false;
    if (LogicMgr::Instance() == nullptr)
        return false;
    return true;
}

bool Kernel::Initialize(int argc, char ** argv) {
    if (!ConfigMgr::Instance()->Initialize(argc, argv)) {
        OASSERT(false, "load config failed");
        return false;
    }

    if (!NetEngine::Instance()->Initialize()) {
        OASSERT(false, "initialize net failed");
        return false;
    }
    if (!LogicMgr::Instance()->Initialize()) {
        OASSERT(false, "initialize net failed");
        return false;
    }
    return true;
}

void Kernel::Loop() {
    while (!_terminate) {
        s64 tick = tools::GetTimeMillisecond();

        NetEngine::Instance()->Loop();
        LogicMgr::Instance()->Loop();

        s64 use = tools::GetTimeMillisecond() - tick;
        if (use > ConfigMgr::Instance()->GetFrameDuration()) {

        }
    }
}

void Kernel::Destroy() {
    LogicMgr::Instance()->Destroy();
    NetEngine::Instance()->Destroy();
    ConfigMgr::Instance()->Destroy();
    DEL this;
}

bool Kernel::Listen(const char * ip, const s32 port, const s32 sendSize, const s32 recvSize, IPacketParser * parser, ISessionFactory * factory) {
    return NetEngine::Instance()->Listen(ip, port, sendSize, recvSize, parser, factory);
}

bool Kernel::Connect(const char * ip, const s32 port, const s32 sendSize, const s32 recvSize, IPacketParser * parser, ISession * session) {
    return NetEngine::Instance()->Connect(ip, port, sendSize, recvSize, parser, session);
}

IModule * Kernel::FindModule(const char * name) {
    return LogicMgr::Instance()->FindModule(name);
}

const char * Kernel::GetCmdArg(const char * key) {
    return ConfigMgr::Instance()->GetCmdArg(key);
}
