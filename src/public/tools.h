#ifndef __TOOLS_H__
#define __TOOLS_H__
#include "util.h"
#include <libgen.h>

namespace tools {
    inline const char * GetAppPath() {
        static char * path = nullptr;

        if (nullptr == path) {
            path = NEW char[256];
            char link[256];
            memset(link, 0, 256);
            memset(path, 0, 256);

            SafeSprintf(link, sizeof (link), "/proc/self/exe");

            int nCount = readlink(link, path, 256);
            if (nCount >= 265) {
                OASSERT(false, "system path error");
            }
            path = dirname(path);
        }
        return path;
    }

    inline s32 StringAsInt(const char * val) {
        OASSERT(val != nullptr, "val is null");
        return atoi(val);
    }

    inline s64 GetTimeMillisecond() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000 + tv.tv_usec/1000;
    }
}
#endif // __TOOLS_H__
