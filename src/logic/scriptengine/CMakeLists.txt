
INCLUDE_DIRECTORIES(
    "${PROJECT_SOURCE_DIR}/src/API"
    "${PROJECT_SOURCE_DIR}/src/public"
    "${PROJECT_SOURCE_DIR}/src/public/tinyxml"
    "${PROJECT_SOURCE_DIR}/src/logic/interface"
    )

SET(LUASRC
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lapi.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lapi.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lauxlib.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lauxlib.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lbaselib.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lbitlib.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lcode.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lcode.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lcorolib.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lctype.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lctype.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/ldblib.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/ldebug.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/ldebug.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/ldo.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/ldo.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/ldump.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lfunc.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lfunc.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lgc.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lgc.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/linit.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/liolib.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/llex.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/llex.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/llimits.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lmathlib.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lmem.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lmem.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/loadlib.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lobject.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lobject.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lopcodes.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lopcodes.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/loslib.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lparser.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lparser.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lprefix.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lstate.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lstate.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lstring.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lstring.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lstrlib.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/ltable.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/ltable.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/ltablib.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/ltm.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/ltm.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lua.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lua.hpp"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/luaconf.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lualib.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lundump.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lundump.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lutf8lib.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lvm.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lvm.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lzio.c"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/lua/lzio.h"
    )

SET(SCRIPTENGINE
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/ScriptEngine.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/ScriptEngine.cpp"
	"${PROJECT_SOURCE_DIR}/src/logic/scriptengine/serialize.h"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/serialize.cpp"
    "${PROJECT_SOURCE_DIR}/src/logic/scriptengine/main.cpp"
    )

SOURCE_GROUP(interface FILES ${LOGIC_INTERFACE})
SOURCE_GROUP(API FILES ${API})
SOURCE_GROUP(public\\tinyxml FILES ${PUBLIC_XML})
SOURCE_GROUP(public FILES ${PUBLIC})
SOURCE_GROUP(lua FILES ${LUASRC})
SOURCE_GROUP(\\ FILES ${SCRIPTENGINE})

ADD_LIBRARY(ScriptEngine SHARED
        ${SCRIPTENGINE}
        ${LUASRC}
        ${API}
        ${LOGIC_INTERFACE}
        ${PUBLIC}
        ${PUBLIC_XML}
        )


