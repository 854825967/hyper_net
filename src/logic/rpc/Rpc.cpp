#include "Rpc.h"
#include "IScriptEngine.h"
#include "IHarbor.h"
#include "NodeProtocol.h"

Rpc * Rpc::s_self = nullptr;
IKernel * Rpc::s_kernel = nullptr;
IScriptEngine * Rpc::s_scriptEngine = nullptr;
IHarbor * Rpc::s_harbor = nullptr;

IScriptModule * Rpc::s_module = nullptr;

bool Rpc::Initialize(IKernel * kernel) {
    s_self = this;
	s_kernel = kernel;

	s_scriptEngine = (IScriptEngine*)kernel->FindModule("ScriptEngine");
	OASSERT(s_scriptEngine, "where is scriptEngine");

	s_harbor = (IHarbor*)kernel->FindModule("Harbor");
	OASSERT(s_harbor, "where is harbor");

	REGPROTOCOL(ANY_NODE, node_proto::RPC, Rpc::OnCall);
	REGPROTOCOL(ANY_NODE, node_proto::RPCNR, Rpc::OnCallNoRet);
	REGPROTOCOL(ANY_NODE, node_proto::RESPONE, Rpc::OnRespone);
	s_harbor->AddNodeListener(this, "Rpc");

    return true;
}

bool Rpc::Launched(IKernel * kernel) {
	s_module = s_scriptEngine->CreateModule("harbor");
	s_scriptEngine->AddModuleFunction(s_module, "call", Rpc::Call);
	s_scriptEngine->AddModuleFunction(s_module, "callNoRet", Rpc::CallNoRet);
	s_scriptEngine->AddModuleFunction(s_module, "typeCall", Rpc::TypeCall);
	s_scriptEngine->AddModuleFunction(s_module, "respone", Rpc::Respone);
    return true;
}

bool Rpc::Destroy(IKernel * kernel) {
	if (s_module)
		s_module->Release();
    DEL this;
    return true;
}

void Rpc::Call(IKernel * kernel, const IScriptArgumentReader * reader, IScriptResultWriter * writer) {
	s32 nodeType = 0;
	s32 nodeId = 0;
	s32 size = 0;
	const void * buffer = nullptr;

	if (!reader->Read("iiS", &nodeType, &nodeId, &buffer, &size))
		return;
	
	s_harbor->PrepareSend(nodeType, nodeId, node_proto::RPC, size);
	s_harbor->Send(nodeType, nodeId, buffer, size);
}

void Rpc::CallNoRet(IKernel * kernel, const IScriptArgumentReader * reader, IScriptResultWriter * writer) {
	s32 nodeType = 0;
	s32 nodeId = 0;
	s32 size = 0;
	const void * buffer = nullptr;

	if (!reader->Read("iiS", &nodeType, &nodeId, &buffer, &size))
		return;

	s_harbor->PrepareSend(nodeType, nodeId, node_proto::RPCNR, size);
	s_harbor->Send(nodeType, nodeId, buffer, size);
}

void Rpc::TypeCall(IKernel * kernel, const IScriptArgumentReader * reader, IScriptResultWriter * writer) {
	s32 nodeType = 0;
	s32 size = 0;
	const void * buffer = nullptr;

	if (!reader->Read("iS", &nodeType, &buffer, &size))
		return;

	s_harbor->PrepareBrocast(nodeType, node_proto::RPCNR, size);
	s_harbor->Brocast(nodeType, buffer, size);
}

void Rpc::Respone(IKernel * kernel, const IScriptArgumentReader * reader, IScriptResultWriter * writer) {
	s32 nodeType = 0;
	s32 nodeId = 0;
	s32 size = 0;
	const void * buffer = nullptr;

	if (!reader->Read("iiS", &nodeType, &nodeId, &buffer, &size))
		return;

	s_harbor->PrepareSend(nodeType, nodeId, node_proto::RESPONE, size);
	s_harbor->Send(nodeType, nodeId, buffer, size);
}

void Rpc::OnOpen(IKernel * kernel, s32 nodeType, s32 nodeId, bool hide, const char * ip, s32 port) {
	s_scriptEngine->Call(s_module, "onOpen", nullptr, "ii", nodeType, nodeId);
}

void Rpc::OnClose(IKernel * kernel, s32 nodeType, s32 nodeId) {
	s_scriptEngine->Call(s_module, "onClose", nullptr, "ii", nodeType, nodeId);
}

void Rpc::OnCall(IKernel * kernel, s32 nodeType, s32 nodeId, const void * context, const s32 size) {
	s_scriptEngine->Call(s_module, "onCall", nullptr, "iiS", nodeType, nodeId, (const char*)context, size);
}

void Rpc::OnCallNoRet(IKernel * kernel, s32 nodeType, s32 nodeId, const void * context, const s32 size) {
	s_scriptEngine->Call(s_module, "onCallNoRet", nullptr, "iiS", nodeType, nodeId, (const char*)context, size);
}

void Rpc::OnRespone(IKernel * kernel, s32 nodeType, s32 nodeId, const void * context, const s32 size) {
	s_scriptEngine->Call(s_module, "onRespone", nullptr, "iiS", nodeType, nodeId, (const char*)context, size);
}
