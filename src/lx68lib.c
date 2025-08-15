/*
	x68000 library
*/


#define lx68lib_c
#define LUA_LIB

#include <stdlib.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

#include <IOCSLIB.H>

#include <BASIC0.H>
#include <AUDIO.H>
#include <GRAPH.H>
#include <MOUSE.h>
#include <SPRITE.H>
#include <STICK.h>

/* porting popen()/pclose() from x68k's libc, for io.popen() */
#include <stdio.h>
/* #include <errno.h> */
/* #include <stdlib.h> */
/* #include <unistd.h> */
#include <string.h>
#include <limits.h>

typedef enum {
    inactive = 0,			/* not opened yet */
    read_mode,				/* read pipe mode */
    write_mode,				/* write pipe mode */
} epmode_t;

typedef struct {
    epmode_t mode;			/* current pipe mode */
    const char *command;		/* command line to execute */
    const char *temporary;		/* temporary file name */
} epipe_t;

/* extern epipe_t _pipes[]; */
epipe_t _pipes[4];

#define EINVAL       14

int pclose (FILE *stream)
{
    int rc;
    char comline[512];
    epipe_t *ep;

    /* パイプデータを取得 */
    ep = &_pipes[stream->_file];

    /* オープンされているか? */
    if (ep->mode == inactive) {
	errno = EINVAL;
	return -1;
    }

    /* 読み込みモードならば... */
    else if (ep->mode == read_mode) {

	/* ファイルをクローズ */
	rc = fclose (stream);

	/* テンポラリファイルを削除 */
	unlink (ep->temporary);

    }

    /* 書き込みモードならば... */
    else {

	/* ファイルをクローズ */
	fclose (stream);

	/* 実行コマンドラインを生成 */
	sprintf (comline, "%s < %s", ep->command, ep->temporary);

	/* コマンドを実行 */
	rc = system (comline);

	/* テンポラリファイルを削除 */
	unlink (ep->temporary);

    }

    /* パイプデータを消去 */
    ep->mode = inactive;

    /* 確保したメモリを解放 */
    free ((void *) ep->command);
    free ((void *) ep->temporary);

    /* rc を返す */
    return rc;
}

FILE *popen (const char *command, const char *mode)
{
    char *temp;
    char comline[512];
    epmode_t pmode;
    epipe_t *ep;
    FILE *fp;

    /* 読み込みモードか? */
    if (mode[0] == 'r' || mode[1] == 'r')
	pmode = read_mode;

    /* 書き込みモードか? */
    else if (mode[0] == 'w' || mode[1] == 'w')
	pmode = write_mode;

    /* 不正なモード */
    else {
	errno = EINVAL;
	return 0;
    }

    /* テンポラリファイルのファイル名を生成する */
    if ((temp = tmpnam (0)) == 0)
	return 0;

    /* 読み込みモードであれば... */
    if (pmode == read_mode) {

	/* コマンドラインを生成する */
	sprintf (comline, "%s > %s", command, temp);

	/* コマンドを実行 */
	system (comline);

	/* 入力ファイルをオープン */
	if ((fp = fopen (temp, mode)) == 0)
	    return 0;

    }

    /* 書き込みモードならば... */
    else {

	/* 出力ファイルをオープン */
	if ((fp = fopen (temp, mode)) == 0)
	    return 0;

    }

    /* パイプの実行データを記録する */
    ep = &_pipes[fp->_file];
    ep->mode = pmode;
    ep->temporary = strdup (temp);
    ep->command = strdup (command);

    /* fp を返す */
    return fp;
}
/* memo: C's return value is number of Lua's return value */

#ifdef LUA_USE_X68KLIB
static int x68k_crtmod (lua_State *L) {
	int ret_value =0;
	const int crtmode = luaL_checkinteger(L, 1);
	const int flag = luaL_optinteger(L, 2, 0);

	ret_value = CRTMOD(crtmode);

	if (flag) {
		G_CLR_ON();
	}

	if (crtmode == -1) {
		lua_pushinteger(L, ret_value);
		return 1;
	}
	
	return 0;
}

static int x68k_curon(lua_State *L) {
	B_CURON();
	return 0;
}

static int x68k_curoff(lua_State *L) {
	B_CUROFF();
	return 0;
}

static int x68k_locate(lua_State *L) {
	const int x = lua_tointeger(L, 1);
	const int y = lua_tointeger(L, 2);
	locate(x, y);
	return 0;
}

static int x68k_cls(lua_State *L) {
	cls();
	return 0;
}

static int x68k_color(lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	color( p1 );
	return 0;
}

static int x68k_vsync(lua_State *L) {
	int oldstat = B_SUPER(0);

	while( (*(volatile unsigned char *)0xE88001 & 0x10) == 0 );
	while( (*(volatile unsigned char *)0xE88001 & 0x10) != 0 );

	B_SUPER(oldstat);

	return 0;
}

static int x68k_SP_REGST(lua_State *L) {
	const int no = lua_tointeger(L, 1);
	const int mode = lua_tointeger(L, 2);
	const int x = lua_tointeger(L, 3);
	const int y = lua_tointeger(L, 4);
	const int code = lua_tointeger(L, 5);
	const int pri = lua_tointeger(L, 6);
	int ret_value;

	if (mode == 0) {	/* wait VSYNC */
		ret_value = SP_REGST( no, x, y, code, pri );
	} else {	/* nowait VSYNC */
		ret_value = SP_REGST( 0x80000000 | no, x, y, code, pri );
	}

	lua_pushinteger(L, ret_value);
	return 1;
}

#if 1
extern int x68k_a_rec (lua_State *L);
extern int x68k_a_stat (lua_State *L);
extern int x68k_a_end (lua_State *L);
extern int x68k_a_stop (lua_State *L);
extern int x68k_a_cont (lua_State *L);
extern int x68k_a_play (lua_State *L);

#else
static int x68k_a_rec (lua_State *L) {
	p1 = malloc(p2);
	if (p1 != NULL) {
		a_rec(p1, p2, p3, p4);
		lua_pushlstring(L, p1, p2);
		free(p1);
	} else {
		lua_pushnil(L);
	}
	return 1;
}

static int x68k_a_stat (lua_State *L) {
	int	ret;
	ret = a_stat();
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_a_end (lua_State *L) {
	a_end();
	return 0;
}

static int x68k_a_stop (lua_State *L) {
	a_stop();
	return 0;
}

static int x68k_a_cont (lua_State *L) {
	a_cont();
	return 0;
}

static int x68k_a_play (lua_State *L) {
	const void *p1 = lua_tostring(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	const int p4 = lua_tointeger(L, 4);
	const int p5 = lua_tointeger(L, 5);
	a_play(p1, p2, p3, p4, p5);
	return 0;
}
#endif

static int x68k_apage (lua_State *L) {
	const char p1 = (char)lua_tointeger(L, 1);
	apage(p1);
	return 0;
}

static int x68k_box (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	const int p4 = lua_tointeger(L, 4);
	const int p5 = lua_tointeger(L, 5);
	const int p6 = luaL_optinteger(L, 6, 65535);
	box(p1, p2, p3, p4, p5, p6);
	return 0;
}

static int x68k_circle (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	const int p4 = lua_tointeger(L, 4);
	const int p5 = lua_tointeger(L, 5);
	const int p6 = lua_tointeger(L, 6);
	const int p7 = lua_tointeger(L, 7);
	circle(p1, p2, p3, p4, p5, p6, p7);
	return 0;
}

static int x68k_contrast (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	contrast(p1);
	return 0;
}

static int x68k_fill (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	const int p4 = lua_tointeger(L, 4);
	const int p5 = lua_tointeger(L, 5);
	fill(p1, p2, p3, p4, p5);
	return 0;
}

static int x68k_get (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	const int p4 = lua_tointeger(L, 4);
	void *p5;
	const int p6 = lua_tointeger(L, 5);

	p5 = malloc(p6);
	get(p1, p2, p3, p4, p5, p6);
	lua_pushlstring( L, (char *)p5, p6);
	free(p5);
	return 1;
}

static int x68k_home (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	home(p1, p2, p3);
	return 0;
}

static int x68k_hsv (lua_State *L) {
	int	ret;
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	ret = hsv(p1, p2, p3);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_line (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	const int p4 = lua_tointeger(L, 4);
	const int p5 = lua_tointeger(L, 5);
	const int p6 = luaL_optinteger(L, 6, 65535);
	line(p1, p2, p3, p4, p5, p6);
	return 0;
}

static int x68k_paint (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	paint(p1, p2, p3);
	return 0;
}

static int x68k_palet (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	palet(p1, p2);
	return 0;
}

static int x68k_point (lua_State *L) {
	int	ret;
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	ret = point(p1, p2);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_pset (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	pset(p1, p2, p3);
	return 0;
}

static int x68k_rgb (lua_State *L) {
	int	ret;
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	ret = rgb(p1, p2, p3);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_vpage (lua_State *L) {
	const char p1 = (char)lua_tointeger(L, 1);
	vpage(p1);
	return 0;
}

static int x68k_window (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	const int p4 = lua_tointeger(L, 4);
	window(p1, p2, p3, p4);
	return 0;
}

static int x68k_wipe (lua_State *L) {
	wipe();
	return 0;
}

static int x68k_put (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	const int p4 = lua_tointeger(L, 4);
	const char *p5 = lua_tostring(L, 5);
	const int p6 = lua_tointeger(L, 6);
	put(p1, p2, p3, p4, (void *)p5, p6);
	return 0;
}

static int x68k_symbol (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const char *p3 = lua_tostring(L, 3);
	const char p4 = (char)lua_tointeger(L, 4);
	const char p5 = (char)lua_tointeger(L, 5);
	const int p6 = lua_tointeger(L, 6);
	const int p7 = lua_tointeger(L, 7);
	const int p8 = lua_tointeger(L, 8);
	symbol(p1, p2, p3, p4, p5, p6, p7, p8);
	return 0;
}

static int x68k_mouse (lua_State *L) {
	int	ret;
	const int p1 = lua_tointeger(L, 1);
	ret = mouse(p1);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_msarea (lua_State *L) {
	int	ret;
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	const int p4 = lua_tointeger(L, 4);
	ret = msarea(p1, p2, p3, p4);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_msbtn (lua_State *L) {
	int	ret;
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	ret = msbtn(p1, p2, p3);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_mspos (lua_State *L) {
	int p1;
	int p2;
	mspos(&p1, &p2);
	lua_pushinteger(L, p1);
	lua_pushinteger(L, p2);
	return 2;
}

static int x68k_msstat (lua_State *L) {
	int p1;
	int p2;
	int p3;
	int p4;
	msstat(&p1, &p2, &p3, &p4);
	lua_pushinteger(L, p1);
	lua_pushinteger(L, p2);
	lua_pushinteger(L, p3);
	lua_pushinteger(L, p4);
	return 4;
}

static int x68k_setmspos (lua_State *L) {
	int	ret;
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	ret = setmspos(p1, p2);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_sp_clr (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	sp_clr(p1, p2);
	return 0;
}

static int x68k_sp_color (lua_State *L) {
	int	ret;
	const char p1 = (char)lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = luaL_optinteger(L, 3, 1);
	ret = sp_color(p1, p2, p3);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_sp_disp (lua_State *L) {
	const char p1 = (char)lua_tointeger(L, 1);
	sp_disp(p1);
	return 0;
}

static int x68k_sp_move (lua_State *L) {
	const char p1 = (char)lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	const int p4 = lua_tointeger(L, 4);
	sp_move(p1, p2, p3, p4);
	return 0;
}

static int x68k_sp_off (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	sp_off(p1, p2);
	return 0;
}

static int x68k_sp_on (lua_State *L) {
	const int p1 = lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	sp_on(p1, p2);
	return 0;
}

static int x68k_sp_pat (lua_State *L) {
	char buf[256];
	const char p1 = (char)lua_tointeger(L, 1);
	const int p3 = lua_tointeger(L, 2);
	sp_pat(p1, buf, p3);
	lua_pushlstring(L, buf, p3==0 ? 64 : 256);
	return 1;
}

static int x68k_sp_init (lua_State *L) {
	int	ret;
	ret = sp_init();
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_sp_set (lua_State *L) {
	int	ret;
	const char p1 = (char)lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	const int p4 = lua_tointeger(L, 4);
	const int p5 = lua_tointeger(L, 5);
	ret = sp_set(p1, p2, p3, p4, p5);
/*
	lua_pushinteger(L, ret);
	return 1;
*/
	return 0;
}

static int x68k_sp_stat (lua_State *L) {
	int	ret;
	const char p1 = (char)lua_tointeger(L, 1);
	const char p2 = (char)lua_tointeger(L, 2);
	ret = sp_stat(p1, p2);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_bg_scroll (lua_State *L) {
	int	ret;
	const char p1 = (char)lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	ret = bg_scroll(p1, p2, p3);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_bg_set (lua_State *L) {
	int	ret;
	const char p1 = (char)lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	const int p3 = lua_tointeger(L, 3);
	ret = bg_set(p1, p2, p3);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_bg_stat (lua_State *L) {
	int	ret;
	const char p1 = (char)lua_tointeger(L, 1);
	const char p2 = (char)lua_tointeger(L, 2);
	ret = bg_stat(p1, p2);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_bg_fill (lua_State *L) {
	int	ret;
	const char p1 = (char)lua_tointeger(L, 1);
	const int p2 = lua_tointeger(L, 2);
	ret = bg_fill(p1, p2);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_bg_put (lua_State *L) {
	int	ret;
	const char p1 = (char)lua_tointeger(L, 1);
	const char p2 = (char)lua_tointeger(L, 2);
	const char p3 = (char)lua_tointeger(L, 3);
	const int p4 = lua_tointeger(L, 4);
	ret = bg_put(p1, p2, p3, p4);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_bg_get (lua_State *L) {
	int	ret;
	const char p1 = (char)lua_tointeger(L, 1);
	const char p2 = (char)lua_tointeger(L, 2);
	const char p3 = (char)lua_tointeger(L, 3);
	ret = bg_get(p1, p2, p3);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_sp_def (lua_State *L) {
	const char p1 = (char)lua_tointeger(L, 1);
	const char *p2 =lua_tostring(L, 2);
	const int p3 = lua_tointeger(L, 3);
	sp_def(p1, p2, p3);
	return 0;
}

static int x68k_stick (lua_State *L) {
	int	ret;
	const int p1 = lua_tointeger(L, 1);
	ret = stick(p1);
	lua_pushinteger(L, ret);
	return 1;
}

static int x68k_strig (lua_State *L) {
	int	ret;
	const int p1 = lua_tointeger(L, 1);
	ret = strig(p1);
	lua_pushinteger(L, ret);
	return 1;
}

static const luaL_Reg x68klib[] = {
	{ "crtmod", x68k_crtmod },
	{ "curon", x68k_curon },
	{ "curoff", x68k_curoff },
	{ "locate", x68k_locate },
	{ "cls", x68k_cls },
	{ "color", x68k_color },
	{ "vsync", x68k_vsync },
	{ "SP_REGST", x68k_SP_REGST },
#if 1
	{ "a_rec", x68k_a_rec },
	{ "a_stat", x68k_a_stat },
	{ "a_end", x68k_a_end },
	{ "a_stop", x68k_a_stop },
	{ "a_cont", x68k_a_cont },
	{ "a_play", x68k_a_play },
#endif
	{ "apage", x68k_apage },
	{ "box", x68k_box },
	{ "circle", x68k_circle },
	{ "contrast", x68k_contrast },
	{ "fill", x68k_fill },
	{ "get", x68k_get },
	{ "home", x68k_home },
	{ "hsv", x68k_hsv },
	{ "line", x68k_line },
	{ "paint", x68k_paint },
	{ "palet", x68k_palet },
	{ "point", x68k_point },
	{ "pset", x68k_pset },
	{ "rgb", x68k_rgb },
	{ "vpage", x68k_vpage },
	{ "window", x68k_window },
	{ "wipe", x68k_wipe },
	{ "put", x68k_put },
	{ "symbol", x68k_symbol },
	{ "mouse", x68k_mouse },
	{ "msarea", x68k_msarea },
	{ "msbtn", x68k_msbtn },
	{ "mspos", x68k_mspos },
	{ "msstat", x68k_msstat },
	{ "setmspos", x68k_setmspos },
	{ "sp_clr", x68k_sp_clr },
	{ "sp_color", x68k_sp_color },
	{ "sp_disp", x68k_sp_disp },
	{ "sp_move", x68k_sp_move },
	{ "sp_off", x68k_sp_off },
	{ "sp_on", x68k_sp_on },
	{ "sp_pat", x68k_sp_pat },
	{ "sp_init", x68k_sp_init },
	{ "sp_set", x68k_sp_set },
	{ "sp_stat", x68k_sp_stat },
	{ "bg_scroll", x68k_bg_scroll },
	{ "bg_set", x68k_bg_set },
	{ "bg_stat", x68k_bg_stat },
	{ "bg_fill", x68k_bg_fill },
	{ "bg_put", x68k_bg_put },
	{ "bg_get", x68k_bg_get },
	{ "sp_def", x68k_sp_def },
	{ "stick", x68k_stick },
	{ "strig", x68k_strig },
	{NULL, NULL}
};

/* }====================================================== */


LUAMOD_API int luaopen_x68k (lua_State *L) {
  luaL_newlib(L, x68klib);
  return 1;
}

#endif

