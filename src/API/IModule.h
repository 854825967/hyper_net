#ifndef __IMODULE_H__
#define __IMODULE_H__
#include "util.h"
#include "IKernel.h"
using namespace core;
#include "tools.h"

#define MODULE_NAME_LEN 64

class IModule {
public:
    virtual ~IModule() {}

    virtual bool Initialize(IKernel * kernel) = 0;
    virtual bool Launched(IKernel * kernel) = 0;
    virtual bool Destroy(IKernel * kernel) = 0;

    virtual void Loop(IKernel * kernel) = 0;

public:
    IModule() {
        _nextModule = nullptr;
        _name[MODULE_NAME_LEN - 1] = 0;
    }

    inline bool SetNext(IModule * next) {
        _nextModule = next;
        return true;
    }

    inline IModule * GetNext() { return _nextModule; }

    inline void SetName(const char * name) {
        SafeSprintf(_name, sizeof(_name) - 1, name);
    }

    inline const char * GetName() { return _name; }

private:
    IModule * _nextModule;
    char _name[MODULE_NAME_LEN];
};

typedef IModule * (*GetModuleFun)(void);

#define NAME_OF_GET_LOGIC_FUN "GetLogicModule"
#define GET_LOGIC_FUN GetLogicModule

#define CREATE_MODULE(name) \
    class factroy##name    \
    {    \
    public:    \
        factroy##name(IModule * & module)    \
        {    \
            IModule * module##name = new name;    \
            module##name->SetName(#name);    \
            module##name->SetNext(module); \
            module = module##name;    \
        }    \
    };    \
    factroy##name factroy##name(g_logicModule);


#define GET_DLL_ENTRANCE \
static IModule * g_logicModule = nullptr; \
extern "C" IModule * GET_LOGIC_FUN() {    \
    srand(tools::GetTimeMillisecond()); \
    return g_logicModule; \
}

#endif // __IMODULE_H__
