#ifndef __LUACORE_H__
#define __LUACORE_H__
#include "util.h"
#include "IScriptEngine.h"
extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
};
#include "IHarbor.h"
#include <functional>
#include <unordered_map>

typedef std::function<void (IKernel * kernel, lua_State * state)> ResultReader;
class ScriptEngine : public IScriptEngine {
public:
    virtual bool Initialize(IKernel * kernel);
    virtual bool Launched(IKernel * kernel);
    virtual bool Destroy(IKernel * kernel);
    virtual void Loop(IKernel * kernel);

	virtual IScriptModule * CreateModule(const char * name);
	virtual bool AddModuleFunction(const IScriptModule * module, const char * func, const ScriptFuncType& f);

	virtual bool Call(const IScriptModule * module, const char * func, const ScriptResultReadFuncType& f, const char * format, ...);

	static ScriptEngine * Self() { return s_self; }

private:
    static bool LoadLua(const char * path, const char * logic);
    static void AddSearchPath(const char* path);

	static s32 Callback(lua_State * state);

	static bool PrepareCall(const IScriptModule * module, const char * func);
    static bool ExecuteFunction(IKernel * kernel, int argc, const ResultReader& reader);
    static bool ExecuteGlobalFunction(IKernel * kernel, const char * functionName, const ResultReader& reader);
    static bool ExecuteString(IKernel * kernel, const char *codes, const ResultReader& reader);
    static bool ExecuteScriptFile(IKernel * kernel, const char * module);

private:
	static ScriptEngine * s_self;
    static IKernel * s_kernel;

    static lua_State * s_state;
};

#endif //__STARTER_H__

