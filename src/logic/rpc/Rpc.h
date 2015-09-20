#ifndef __RPC_H__
#define __RPC_H__
#include "util.h"
#include "IModule.h"
#include "IHarbor.h"

class IScriptEngine;
class IScriptArgumentReader;
class IScriptResultWriter;
class IScriptModule;
class Rpc : public IModule, public INodeListener {
public:
    virtual bool Initialize(IKernel * kernel);
    virtual bool Launched(IKernel * kernel);
    virtual bool Destroy(IKernel * kernel);
    virtual void Loop(IKernel * kernel) {}

	static Rpc * Self() { return s_self; }

	static void Call(IKernel * kernel, const IScriptArgumentReader * reader, IScriptResultWriter * writer);
	static void CallNoRet(IKernel * kernel, const IScriptArgumentReader * reader, IScriptResultWriter * writer);
	static void TypeCall(IKernel * kernel, const IScriptArgumentReader * reader, IScriptResultWriter * writer);
	static void Respone(IKernel * kernel, const IScriptArgumentReader * reader, IScriptResultWriter * writer);

	virtual void OnOpen(IKernel * kernel, s32 nodeType, s32 nodeId, bool hide, const char * ip, s32 port);
	virtual void OnClose(IKernel * kernel, s32 nodeType, s32 nodeId);

	static void OnCall(IKernel * kernel, s32 nodeType, s32 nodeId, const void * context, const s32 size);
	static void OnCallNoRet(IKernel * kernel, s32 nodeType, s32 nodeId, const void * context, const s32 size);
	static void OnRespone(IKernel * kernel, s32 nodeType, s32 nodeId, const void * context, const s32 size);

private:
	static Rpc * s_self;
    static IKernel * s_kernel;
	static IScriptEngine * s_scriptEngine;
	static IHarbor * s_harbor;

	static IScriptModule * s_module;
};

#endif //__RPC_H__

