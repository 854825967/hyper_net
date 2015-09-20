#include "serialize.h"
#include <string.h>
#include <stdlib.h>

#define BUFF_SIZE 2048

class SerializeStream
{
public:
	SerializeStream(char * buffer, size_t len) : _buffer(buffer), _len(len), _offset(0) {}
	SerializeStream() : _len(BUFF_SIZE), _offset(0) { _buffer = (char*)MALLOC(BUFF_SIZE); }
	
	void release() { if (_buffer) FREE(_buffer); }
	
	void * getBuffer() { return (void *)_buffer; }
	size_t len() { return _offset; }
	
	bool read(void * dst, s32 len) {
		if (_offset + len > _len)
			return false;
		memcpy(dst, _buffer + _offset, len);
		_offset += len;
        return true;
	}
	
	template <typename T>
	bool read(T& t) {
		return read((char*)&t, sizeof(T));
	}
	
	bool readString(const char *& str, s32& sz) {
		s32 len = 0;
		if (!read(len))
			return false;
		
		sz = len;
		str = (const char*)(_buffer + _offset);
		_offset += len;
        return true;
	}
	
	bool write(const char * src, s32 len) {
		if (_offset + len > _len)
			return false;

		memcpy(_buffer + _offset, src, len);
		_offset += len;
        return true;
	}
	
	template <typename T>
	bool write(T t) {
		return write((const char*)&t, sizeof(T)); 
	}
	
	bool writeString(const char * src, s32 sz) {
		s32 len = sz;
		if (!write(len))
			return false;
			
		memcpy(_buffer + _offset, src, len);
		_offset += len;
        return true;
	}
	
private:
	char * _buffer;
	s32 _len;
	s32 _offset;
};

namespace lseri {
	const s8 NIL = 0;
	const s8 BOOLEAN = 1;
	const s8 INT = 2;
	const s8 DOUBLE = 3;
	const s8 STRING = 4;
	const s8 TABLE = 5;
}

using namespace lseri;

void pack_one(lua_State * L, SerializeStream& s, s32 index);

void pack_nil(lua_State * L, SerializeStream& s) {
	if (!s.write(NIL)) {
		s.release();
		luaL_error(L, "serialize buffer full");
	}
}

void pack_integer(lua_State * L, SerializeStream& s, s64 n) {
	if (!s.write(INT)) {
		s.release();
		luaL_error(L, "serialize buffer full");
	}

	if (!s.write(n)) {
		s.release();
		luaL_error(L, "serialize buffer full");
	}
}

void pack_double(lua_State * L, SerializeStream& s, double n) {
	if (!s.write(DOUBLE)) {
		s.release();
		luaL_error(L, "serialize buffer full");
	}
	if (!s.write(n)) {
		s.release();
		luaL_error(L, "serialize buffer full");
	}
}

void pack_number(lua_State * L, SerializeStream& s, s32 index) {
	if (lua_isinteger(L, index)) {
		lua_Integer x = lua_tointeger(L, index);
		pack_integer(L, s, x);
	}
	else {
		lua_Number x = lua_tonumber(L, index);
		pack_double(L, s, x);
	}
}

void pack_boolean(lua_State * L, SerializeStream& s, s32 index) {
	bool b = lua_toboolean(L, index);
	if (!s.write(BOOLEAN)) {
		s.release();
		luaL_error(L, "serialize buffer full");
	}

	if (!s.write(b)) {
		s.release();
		luaL_error(L, "serialize buffer full");
	}
}

void pack_string(lua_State * L, SerializeStream& s, s32 index) {
	size_t sz = 0;
	const char *str = lua_tolstring(L, index, &sz);
	if (!s.write(STRING)) {
		s.release();
		luaL_error(L, "serialize buffer full");
	}
	if (!s.writeString(str, sz)) {
		s.release();
		luaL_error(L, "serialize buffer full");
	}	
}

void pack_table(lua_State * L, SerializeStream& s, s32 index) {
	if (!s.write(TABLE)) {
		s.release();
		luaL_error(L, "serialize buffer full");
	}
	lua_pushnil(L);
	while (lua_next(L, index) != 0) {
		pack_one(L, s, -2);
		pack_one(L, s, -1);
		lua_pop(L, 1);
	}
	pack_nil(L, s);
}

void pack_one(lua_State * L, SerializeStream& s, s32 index) {
	s32 type = lua_type(L, index);
	switch (type) {
		case LUA_TNIL: pack_nil(L, s); break;
		case LUA_TNUMBER: pack_number(L, s, index); break;
		case LUA_TBOOLEAN: pack_boolean(L, s, index); break;
		case LUA_TSTRING: pack_string(L, s, index); break;
		case LUA_TTABLE: {
			if (index < 0) {
				index = lua_gettop(L) + index + 1;
			}
			pack_table(L, s, index);
			break;
		}
		default: {
			s.release();
			luaL_error(L, "Unsupport type %s to serialize", lua_typename(L, type));
		}
	}
}

static s32 l_serialize(lua_State * L)
{
	SerializeStream s;
	
	s32 n = lua_gettop(L);
	for (s32 i = 1; i <= n; ++i) {
		pack_one(L, s, i);
	}
	
	lua_pushlstring(L, (const char *)s.getBuffer(), s.len());
	s.release();
	return 1;
}

void push_one(lua_State * L, SerializeStream& s);

void push_table(lua_State * L, SerializeStream& s) {
	lua_newtable(L);
	while (true) {
		push_one(L, s);
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			return;
		}
		push_one(L, s);
		lua_rawset(L, -3);
	}
}

void push_value(lua_State * L, SerializeStream& s, s8 type) {
	switch (type) {
		case NIL: lua_pushnil(L); break;
		case BOOLEAN: {
			bool value;
			if (!s.read(value))
				luaL_error(L, "unserialize failed");
			lua_pushboolean(L, value); 
			break;
		}
		case INT: {
			s64 value;
			if (!s.read(value))
				luaL_error(L, "unserialize failed");
			lua_pushinteger(L, value); 
			break;
		}
		case DOUBLE: {
			double value;
			if (!s.read(value))
				luaL_error(L, "unserialize failed");
			lua_pushnumber(L, value); 
			break;
		}
		case STRING: {
			s32 len = 0; 
			const char * str = NULL;
			if (!s.readString(str, len)) 
				luaL_error(L, "unserialize failed");
			lua_pushlstring(L, str, len); 
			break;
		}
		case TABLE: push_table(L, s); break;
		default: 
			luaL_error(L, "Unsupport type %d to unserialize", type);
	}
}

void push_one(lua_State * L, SerializeStream& s) {
	s8 type;
	if (!s.read(type))
		luaL_error(L, "unserialize failed");
	push_value(L, s, type);
}

static s32 l_unserialize(lua_State * L) {
	if (lua_isnoneornil(L,1)) {
		return 0;
	}
	
	size_t len;
	const char* buffer = lua_tolstring(L, 1, &len);
	
	if (len == 0)
		return 0;
	
	if (!buffer)
		return luaL_error(L, "unserialize null pointer");
	
	SerializeStream s((char*)buffer, len); 

	lua_settop(L, 0);
	
	for (s32 i=0; ; ++i) {
		if (i % 16 == 15)
			lua_checkstack(L, i);
		
		s8 type;
		if (!s.read(type))
			break;
		push_value(L, s, type);
	}

	return lua_gettop(L);
}

static s32 l_tohex(lua_State * L) {
	if (lua_isnoneornil(L,1))
		return 0;
	
	size_t len;
	char* buffer = (char*)lua_tolstring(L, 1, &len);
	
	if (!buffer)
		return luaL_error(L, "tohex null pointer");
	
	if (len == 0) 
		lua_pushstring(L, ""); 
	else {
		char * hex = (char *)MALLOC(len * 2 + 3);
		char * cur = hex;
		*cur++ = '0';
		*cur++ = 'x';
		for (s32 i = 0; i < len; ++i)
		{
			char low = *(buffer + i) & 0x0F;
			char high = (*(buffer + i) & 0xF0) >> 4;
			*cur++ = ((high < 10) ? high + '0' : high + 'A' - 10);
			*cur++ = ((low < 10) ? low + '0' : low + 'A' - 10);
		}
		*cur = 0;
		
		lua_pushstring(L, hex); 
		FREE(hex);
	}
	
	return 1;
}

static const luaL_Reg R[] = {
	{"serialize",	l_serialize},
	{"unserialize",	l_unserialize},
	{"tohex",	l_tohex},
	{NULL,	NULL}
};

s32 luaopen_seri(lua_State *L) {
	lua_getglobal(L, LUA_STRLIBNAME);
	luaL_setfuncs(L, R, 0);
	lua_pop(L, 1);
	return 0;
}
