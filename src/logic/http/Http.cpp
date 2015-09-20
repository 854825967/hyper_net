#include "Http.h"
#include "IScriptEngine.h"
#include "tinyxml.h"

Http * Http::s_self = nullptr;
IKernel * Http::s_kernel = nullptr;
IScriptEngine * Http::s_scriptEngine = nullptr;

IScriptModule * Http::s_module = nullptr;

class HttpRunner : public olib::IRunner<HttpCommand> {
public:
	HttpRunner() {}
	virtual ~HttpRunner() {}

	virtual bool Execute(HttpCommand * command) {

	}

	virtual void Release() {
		DEL this;
	}

private:

};

bool Http::Initialize(IKernel * kernel) {
    s_self = this;
    s_kernel = kernel;

	s_scriptEngine = (IScriptEngine*)kernel->FindModule("ScriptEngine");
	OASSERT(s_scriptEngine, "where is scriptEngine");

    return true;
}

bool Http::Launched(IKernel * kernel) {
	s_module = s_scriptEngine->CreateModule("http");
	OASSERT(s_module, "create module failed");
	s_scriptEngine->AddModuleFunction(s_module, "get", Http::Get);
	s_scriptEngine->AddModuleFunction(s_module, "post", Http::Post);

	TiXmlDocument doc;
	std::string coreConfigPath = std::string(tools::GetAppPath()) + "/config/server_conf.xml";
	if (!doc.LoadFile(coreConfigPath.c_str())) {
		OASSERT(false, "can't find core file : %s", coreConfigPath.c_str());
		return false;
	}

	const TiXmlElement * pRoot = doc.RootElement();
	OASSERT(pRoot != nullptr, "core xml format error");

	const TiXmlElement * p = pRoot->FirstChildElement("http");
	s32 threadCount = tools::StringAsInt(p->Attribute("thread"));

	if (!Start(threadCount, this)) {
		OASSERT(false, "wtf");
		return false;
	}

    return true;
}

bool Http::Destroy(IKernel * kernel) {
	Terminate();
    DEL this;
    return true;
}

void Http::Get(IKernel * kernel, const IScriptArgumentReader * reader, IScriptResultWriter * writer) {
	s64 threadId = 0;
	s64 sequenceId = 0;
	const char * uri = nullptr;
	if (!reader->Read("lls", &threadId, &sequenceId, &uri)) {
		OASSERT(false, "read uri failed");
		return;
	}

	HttpCommand * command = NEW HttpCommand;
	memset(command, 0, sizeof(HttpCommand));

	command->sequenceId = sequenceId;
	SafeSprintf(command->url, sizeof(command->url), uri);
	command->method = METHOD_GET;

	s_self->Push(threadId, command);
}

void Http::Post(IKernel * kernel, const IScriptArgumentReader * reader, IScriptResultWriter * writer) {
	s64 threadId = 0;
	s64 sequenceId = 0;
	const char * url = nullptr;
	const char * param = nullptr;
	if (!reader->Read("llss", &threadId, &sequenceId, &url, &param)) {
		OASSERT(false, "read uri failed");
		return;
	}

	HttpCommand * command = NEW HttpCommand;
	memset(command, 0, sizeof(HttpCommand));

	command->sequenceId = sequenceId;
	SafeSprintf(command->url, sizeof(command->url), url);
	SafeSprintf(command->param, sizeof(command->param), param);
	command->method = METHOD_POST;

	s_self->Push(threadId, command);
}

void Http::Complete(HttpCommand * command) {
	if (command->errCode == NO_ERROR)
		s_scriptEngine->Call(s_module, "onSuccess", nullptr, "ls", command->sequenceId, command->content);
	else 
		s_scriptEngine->Call(s_module, "OnFailed", nullptr, "ls", command->sequenceId, command->content);
}

olib::IRunner<HttpCommand> * Http::Create() {
	return NEW HttpRunner;
}
