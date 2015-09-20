#include "LogicMgr.h"
#include "IModule.h"
#include "kernel.h"
#include "tinyxml/tinyxml.h"
#include "ConfigMgr.h"
#include "tools.h"

bool LogicMgr::Ready() {
    return true;
}

bool LogicMgr::Initialize() {
    const char * name = ConfigMgr::Instance()->GetCmdArg("name");
    OASSERT(name, "invalid command args, there is no name");

    TiXmlDocument doc;
    std::string coreConfigPath = std::string(tools::GetAppPath()) + "/config/extend/" + name + ".xml";
    if (!doc.LoadFile(coreConfigPath.c_str())) {
        OASSERT(false, "can't find core file : %s", coreConfigPath.c_str());
        return false;
    }

    const TiXmlElement * pRoot = doc.RootElement();
    OASSERT(pRoot, "module.xml format error");
    const char * path = pRoot->Attribute("path");
    OASSERT(path, "module.xml format error, can't find module path");

    const TiXmlElement * module = pRoot->FirstChildElement("module");
    while (module) {
        const char * name = module->Attribute("name");
        OASSERT(name, "module.xml form error, can't find module's name");

        char fullPath[512];
        SafeSprintf(fullPath, sizeof(fullPath), "%s/%slib%s.so", tools::GetAppPath(), path, name);
        if (!LoadModule(fullPath)) {
            OASSERT(false, "load module failed");
            return false;
        }

        module = module->NextSiblingElement("module");
    }

    for (auto itr = _modules.begin(); itr != _modules.end(); ++itr)
        itr->second->Initialize(Kernel::Instance());

    for (auto itr = _modules.begin(); itr != _modules.end(); ++itr)
        itr->second->Launched(Kernel::Instance());

    return true;
}

void LogicMgr::Loop() {
    for (auto itr = _modules.begin(); itr != _modules.end(); ++itr)
        itr->second->Loop(Kernel::Instance());
}

void LogicMgr::Destroy() {
    for (auto itr = _modules.begin(); itr != _modules.end(); ++itr)
        itr->second->Destroy(Kernel::Instance());
    DEL this;
}

bool LogicMgr::LoadModule(const char * path) {
    void * handle = dlopen(path, RTLD_LAZY);
    OASSERT(handle, "open %s error %s", path, dlerror());

    GetModuleFun fun = (GetModuleFun) dlsym(handle, NAME_OF_GET_LOGIC_FUN);
    OASSERT(fun, "get function:GetLogicModule error");

    IModule * logic = fun();
    OASSERT(logic, "can't get module from %s", path);

    while (logic) {
        const char * name = logic->GetName();
        auto itr = _modules.find(name);
        if (itr != _modules.end()) {
            OASSERT(false, "duplicate logic name %s", name);
            return false;
        }

        _modules.insert(std::make_pair(name, logic));

        logic = logic->GetNext();
    }
    return true;
}
