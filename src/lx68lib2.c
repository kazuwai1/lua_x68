/*
	X68000 lib 2 (without compiling option'-O')
*/

#define lx68lib_c
#define LUA_LIB

#include <stdlib.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

#include <IOCSLIB.H>

#include <AUDIO.H>

/* memo: C's return value is number of Lua's return value */

#ifdef LUA_USE_X68KLIB
int x68k_a_rec (lua_State *L) {
	const int p2 = lua_tointeger(L, 1);
	const int p3 = lua_tointeger(L, 2);
	char *p1;
	p1 = malloc(p2);
	if (p1 != NULL) {
		a_rec(p1, p2, p3, p2);
		lua_pushlstring(L, p1, p2);
		free(p1);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

int x68k_a_stat (lua_State *L) {
	int	ret;
	ret = a_stat();
	lua_pushinteger(L, ret);
	return 1;
}

int x68k_a_end (lua_State *L) {
	a_end();
	return 0;
}

int x68k_a_stop (lua_State *L) {
	a_stop();
	return 0;
}

int x68k_a_cont (lua_State *L) {
	a_cont();
	return 0;
}

int x68k_a_play (lua_State *L) {
	const void *p1 = lua_tostring(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	const int p4 = lua_tointeger(L, 4);
	const int p5 = lua_tointeger(L, 5);
	a_play(p1, p2, p3, p4, p5);
	return 0;
}

#endif
