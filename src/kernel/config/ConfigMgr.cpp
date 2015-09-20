#include "ConfigMgr.h"
#include "tools.h"
#include "tinyxml.h"

ConfigMgr::ConfigMgr() {
    //ctor
}

ConfigMgr::~ConfigMgr() {
    //dtor
}


bool ConfigMgr::Ready() {
    return true;
}

bool ConfigMgr::Initialize(int argc, char ** argv) {
    if (!parse(argc, argv))
        return false;

    const char * name = GetCmdArg("name");
    OASSERT(name, "invalid command args, there is no name");

    TiXmlDocument doc;
    std::string coreConfigPath = std::string(tools::GetAppPath()) + "/config/server_conf.xml";
    if (!doc.LoadFile(coreConfigPath.c_str())) {
        OASSERT(false, "can't find core file : %s", coreConfigPath.c_str());
        return false;
    }

    const TiXmlElement * pRoot = doc.RootElement();
    OASSERT(pRoot != nullptr, "core xml format error");

    const TiXmlElement * p = pRoot->FirstChildElement("loop");
    OASSERT(p != nullptr, "core xml format error, can't find child loop");
    _frameDuration = tools::StringAsInt(p->Attribute("tick"));

    p = pRoot->FirstChildElement("net");
    OASSERT(p != nullptr, "core xml format error, can't find child netthread");
    _netFrameTick = tools::StringAsInt(p->Attribute("tick"));
    _netFrameWaitTick = tools::StringAsInt(p->Attribute("wait"));
    _netSupportSize = tools::StringAsInt(p->Attribute("support"));
	_netThreadCount = tools::StringAsInt(p->Attribute("thread"));
    return true;
}

void ConfigMgr::Destroy() {
    DEL this;
}

const char * ConfigMgr::GetCmdArg(const char * name) {
    auto itr = _args.find(name);
    if (itr != _args.end()) {
        return itr->second.c_str();
    }
    return nullptr;
}

bool ConfigMgr::parse(int argc, char ** argv) {
    for (s32 i = 1; i < argc; ++i) {
        OASSERT(strncmp(argv[i], "--", 2) == 0, "invalid argv %s", argv[i]);

        char * start = argv[i] + 2;
        char * equal = strstr(start, "=");
        OASSERT(equal, "invalid argv %s", argv[i]);
        std::string name(start, equal);
        std::string val(equal + 1);
        _args[name] = val;
    }
    return true;
}
