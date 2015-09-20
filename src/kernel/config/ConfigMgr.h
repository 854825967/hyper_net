#ifndef __CONFIGMGR_H__
#define __CONFIGMGR_H__
#include "util.h"
#include "singleton.h"
#include <string>
#include <unordered_map>

class ConfigMgr : public OSingleton<ConfigMgr> {
    friend class OSingleton<ConfigMgr>;
public:
    bool Ready();
    bool Initialize(int argc, char ** argv);
    void Destroy();

    const char * GetCmdArg(const char * name);

    inline s32 GetFrameDuration() const { return _frameDuration; }

    inline s32 GetNetFrameTick() const { return _netFrameTick; }
    inline s32 GetNetFrameWaitTick() const { return _netFrameWaitTick; }
    inline s32 GetNetSupportSize() const { return _netSupportSize; }
	inline s32 GetNetThreadCount() const { return _netThreadCount; }

private:
    bool parse(int argc, char ** argv);

private:
    ConfigMgr();
    virtual ~ConfigMgr();

    std::unordered_map<std::string, std::string> _args;

    s32 _frameDuration;

    s32 _netFrameTick;
    s32 _netFrameWaitTick;
    s32 _netSupportSize;
	s32 _netThreadCount;
};

#endif // __CONFIGMGR_H__
