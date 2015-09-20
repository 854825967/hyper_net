#ifndef LSERI_H_
#define LSERI_H_
#include "util.h"
extern "C" {
	#include "lua/lua.h"
	#include "lua/lualib.h"
	#include "lua/lauxlib.h"
};

s32 luaopen_seri(lua_State *L);

#endif /* LSERI_H_ */
