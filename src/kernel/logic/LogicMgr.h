#ifndef __LOGICMGR_H__
#define __LOGICMGR_H__
#include "util.h"
#include "singleton.h"
#include <unordered_map>

class IModule;
class LogicMgr : public OSingleton<LogicMgr> {
    friend class OSingleton<LogicMgr>;
public:
    bool Ready();
    bool Initialize();
    void Loop();
    void Destroy();

    inline IModule * FindModule(const char * name) {
        auto itr = _modules.find(name);
        if (itr != _modules.end())
            return itr->second;
        return nullptr;
    }

private:
    LogicMgr() {}
    ~LogicMgr() {}

    bool LoadModule(const char * name);

private:
    std::unordered_map<std::string, IModule *> _modules;
};

#endif //__LOGICMGR_H__
