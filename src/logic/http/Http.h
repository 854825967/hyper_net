#ifndef __HTTP_H__
#define __HTTP_H__
#include "util.h"
#include "IModule.h"
#include "ThreadModule.h"

#define MAX_URL_SIZE 256
#define MAX_PARAM_SIZE 1024

enum {
	METHOD_GET,
	METHOD_POST,
};

#define NO_ERROR 0

struct HttpCommand {
	s64 sequenceId;
	char url[MAX_URL_SIZE];
	char param[MAX_PARAM_SIZE];
	s8 method;
	s32 errCode;
	char * content;
};

class IScriptEngine;
class IScriptModule;
class IScriptArgumentReader;
class IScriptResultWriter;
class Http : public IModule, public olib::ThreadModule<HttpCommand>, public olib::IRunnerFactory<HttpCommand> {
public:
    virtual bool Initialize(IKernel * kernel);
    virtual bool Launched(IKernel * kernel);
    virtual bool Destroy(IKernel * kernel);
    virtual void Loop(IKernel * kernel) {}

	static Http * Self() { return s_self; }

	static void Get(IKernel * kernel, const IScriptArgumentReader * reader, IScriptResultWriter * writer);
	static void Post(IKernel * kernel, const IScriptArgumentReader * reader, IScriptResultWriter * writer);

	virtual void Complete(HttpCommand * command);

	virtual olib::IRunner<HttpCommand> * Create();

private:
	static Http * s_self;
    static IKernel * s_kernel;
	static IScriptEngine * s_scriptEngine;

	static IScriptModule * s_module;
};

#endif //__HTTP_H__

