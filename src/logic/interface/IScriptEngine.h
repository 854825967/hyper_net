#ifndef __ISCRIPTENGINE_H__
#define __ISCRIPTENGINE_H__

#include "IKernel.h"
#include "IModule.h"
#include <functional>

class IScriptArgumentReader {
public:
	virtual ~IScriptArgumentReader() {}

	virtual bool Read(const char * format, ...) const = 0;
};

class IScriptResultWriter {
public:
	virtual ~IScriptResultWriter() {}

	virtual void Write(const char * format, ...) = 0;
};

typedef void(*ScriptFuncType)(IKernel * kernel, const IScriptArgumentReader * reader, IScriptResultWriter * writer);

class IScriptCallResult {
public:
	virtual ~IScriptCallResult() {}

	virtual bool Read(const char * format, ...) const = 0;
};

typedef std::function<void(IKernel * pKernel, IScriptCallResult *)> ScriptResultReadFuncType;
class IScriptModule {
public:
	virtual ~IScriptModule() {}

	virtual void Release() = 0;
};

class IScriptEngine : public IModule {
public:
	virtual ~IScriptEngine() {}

	virtual IScriptModule * CreateModule(const char * name) = 0;
	virtual bool AddModuleFunction(const IScriptModule * module, const char * func, const ScriptFuncType& f) = 0;

	virtual bool Call(const IScriptModule * module, const char * func, const ScriptResultReadFuncType& f, const char * format, ...) = 0;
};


#endif //__ISCRIPTENGINE_H__
