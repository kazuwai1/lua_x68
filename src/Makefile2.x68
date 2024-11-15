# for INSTALL
TO_BIN= lua.x luac.x
TO_INC= lua.h luaconf.h lualib.h lauxlib.h lua.hpp
TO_LIB= liblua.a
TO_MAN= lua.1 luac.1

# Ver
V= 5.4
R= $V.7

# for compile...
CC= gcc2
CFLAGS= $(SYSCFLAGS) $(MYCFLAGS)
LDFLAGS= $(SYSLDFLAGS) $(MYLDFLAGS)
LIBS= $(SYSLIBS) $(MYLIBS)

AR= oar
RANLIB=
RM= rm -f
UNAME= uname

SYSCFLAGS=
SYSLDFLAGS=
SYSLIBS=

MYCFLAGS= -O2 -fomit-frame-pointer -fstrength-reduce -finline-functions -m68000
MYLDFLAGS= 
MYLIBS= a:/lib/libgcc.a a:/lib/floatfnc.l a:/lib/clib.l a:/lib/iocslib.l a:/lib/baslib.l a:/lib/gnulib.l
MYOBJS=

LUA_A= lualib.l
CORE_O= lapi.o lcode.o lctype.o ldebug.o ldo.o ldump.o lfunc.o lgc.o llex.o lmem.o lobject.o lopcodes.o lparser.o lstate.o lstring.o ltable.o ltm.o lundump.o lzio.o
CORE_O2= lvm.o
LIB_O= lauxlib.o lbaselib.o lcorolib.o ldblib.o liolib.o lmathlib.o loadlib.o loslib.o lstrlib.o ltablib.o lutf8lib.o linit.o lx68lib.o lx68lib2.o
BASE_O= $(CORE_O) $(CORE_O2) $(LIB_O) $(MYOBJS)

LUA_T= lua.x
LUA_O= lua.o

LUAC_T= luac.x
LUAC_O= luac.o

ALL_O= $(BASE_O) $(LUA_O) $(LUAC_O)
ALL_T= $(LUA_A) $(LUA_T) $(LUAC_T)
ALL_A= $(LUA_A)

all: $(ALL_T)

all_o: $(ALL_O)

all_a: $(ALL_A)

$(LUA_T): $(LUA_O) $(LUA_A)
	$(CC) -z-heap=1048576 -o $@ $(LDFLAGS) $(LUA_O) $(LUA_A) $(LIBS)

$(LUAC_T): $(LUAC_O) $(LUA_A)
	$(CC) -o $@ $(LDFLAGS) $(LUAC_O) $(LUA_A) $(LIBS)

$(LUA_A): $(BASE_O)
	$(AR) -c $@ $(BASE_O)

llex.o:
	$(CC) $(CFLAGS) -c llex.c

lparser.o:
	$(CC) $(CFLAGS) -c lparser.c

lcode.o:
	$(CC) $(CFLAGS) -c lcode.c

lvm.o: lvm.c
	$(CC) $(CFLAGS) -cc1-stack=262144 -c lvm.c

lx68lib2.o: lx68lib2.c
	$(CC) -c lx68lib2.c

.c.o:
	$(CC) $(CFLAGS) -c $<

ldblib.o : ldblib.c lprefix.h lua.h luaconf.h lauxlib.h lualib.h 
lmathlib.o : lmathlib.c lprefix.h lua.h luaconf.h lauxlib.h lualib.h 
loslib.o : loslib.c lprefix.h lua.h luaconf.h lauxlib.h lualib.h 
loadlib.o : loadlib.c lprefix.h lua.h luaconf.h lauxlib.h lualib.h 
lmem.o : lmem.c lprefix.h lua.h luaconf.h ldebug.h lstate.h lobject.h \
  llimits.h ltm.h lzio.h lmem.h ldo.h lgc.h 
lopcodes.o : lopcodes.c lprefix.h lopcodes.h llimits.h lua.h luaconf.h 
lua.o : lua.c lprefix.h lua.h luaconf.h lauxlib.h lualib.h 
lbaselib.o : lbaselib.c lprefix.h lua.h luaconf.h lauxlib.h lualib.h 
ltable.o : ltable.c lprefix.h lua.h luaconf.h ldebug.h lstate.h lobject.h \
  llimits.h ltm.h lzio.h lmem.h ldo.h lgc.h lstring.h ltable.h lvm.h 
ldump.o : ldump.c lprefix.h lua.h luaconf.h lobject.h llimits.h lstate.h \
  ltm.h lzio.h lmem.h lundump.h 
liolib.o : liolib.c lprefix.h lua.h luaconf.h lauxlib.h lualib.h 
lzio.o : lzio.c lprefix.h lua.h luaconf.h llimits.h lmem.h lstate.h \
  lobject.h ltm.h lzio.h 
lctype.o : lctype.c lprefix.h lctype.h lua.h luaconf.h llimits.h 
lstring.o : lstring.c lprefix.h lua.h luaconf.h ldebug.h lstate.h lobject.h \
  llimits.h ltm.h lzio.h lmem.h ldo.h lstring.h lgc.h 
llex.o : llex.c lprefix.h lua.h luaconf.h lctype.h llimits.h ldebug.h \
  lstate.h lobject.h ltm.h lzio.h lmem.h ldo.h lgc.h llex.h lparser.h \
  lstring.h ltable.h 
linit.o : linit.c lprefix.h lua.h luaconf.h lualib.h lauxlib.h 
ldebug.o : ldebug.c lprefix.h lua.h luaconf.h lapi.h llimits.h lstate.h \
  lobject.h ltm.h lzio.h lmem.h lcode.h llex.h lopcodes.h lparser.h ldebug.h \
  ldo.h lfunc.h lstring.h lgc.h ltable.h lvm.h 
ldo.o : ldo.c lprefix.h lua.h luaconf.h lapi.h llimits.h lstate.h lobject.h \
  ltm.h lzio.h lmem.h ldebug.h ldo.h lfunc.h lgc.h lopcodes.h lparser.h \
  lstring.h ltable.h lundump.h lvm.h 
lauxlib.o : lauxlib.c lprefix.h lua.h luaconf.h lauxlib.h 
luac.o : luac.c lprefix.h lua.h luaconf.h lauxlib.h ldebug.h lstate.h \
  lobject.h llimits.h ltm.h lzio.h lmem.h lopcodes.h lopnames.h lundump.h 
lcorolib.o : lcorolib.c lprefix.h lua.h luaconf.h lauxlib.h lualib.h 
lutf8lib.o : lutf8lib.c lprefix.h lua.h luaconf.h lauxlib.h lualib.h 
lgc.o : lgc.c lprefix.h lua.h luaconf.h ldebug.h lstate.h lobject.h \
  llimits.h ltm.h lzio.h lmem.h ldo.h lfunc.h lgc.h lstring.h ltable.h 
lstate.o : lstate.c lprefix.h lua.h luaconf.h lapi.h llimits.h lstate.h \
  lobject.h ltm.h lzio.h lmem.h ldebug.h ldo.h lfunc.h lgc.h llex.h lstring.h \
  ltable.h 
lundump.o : lundump.c lprefix.h lua.h luaconf.h ldebug.h lstate.h lobject.h \
  llimits.h ltm.h lzio.h lmem.h ldo.h lfunc.h lstring.h lgc.h lundump.h 
ltablib.o : ltablib.c lprefix.h lua.h luaconf.h lauxlib.h lualib.h 
ltm.o : ltm.c lprefix.h lua.h luaconf.h ldebug.h lstate.h lobject.h \
  llimits.h ltm.h lzio.h lmem.h ldo.h lgc.h lstring.h ltable.h lvm.h 
lparser.o : lparser.c lprefix.h lua.h luaconf.h lcode.h llex.h lobject.h \
  llimits.h lzio.h lmem.h lopcodes.h lparser.h ldebug.h lstate.h ltm.h ldo.h \
  lfunc.h lstring.h lgc.h ltable.h 
lobject.o : lobject.c lprefix.h lua.h luaconf.h lctype.h llimits.h ldebug.h \
  lstate.h lobject.h ltm.h lzio.h lmem.h ldo.h lstring.h lgc.h lvm.h 
lcode.o : lcode.c lprefix.h lua.h luaconf.h lcode.h llex.h lobject.h \
  llimits.h lzio.h lmem.h lopcodes.h lparser.h ldebug.h lstate.h ltm.h ldo.h \
  lgc.h lstring.h ltable.h lvm.h 
lfunc.o : lfunc.c lprefix.h lua.h luaconf.h ldebug.h lstate.h lobject.h \
  llimits.h ltm.h lzio.h lmem.h ldo.h lfunc.h lgc.h 
lapi.o : lapi.c lprefix.h lua.h luaconf.h lapi.h llimits.h lstate.h \
  lobject.h ltm.h lzio.h lmem.h ldebug.h ldo.h lfunc.h lgc.h lstring.h \
  ltable.h lundump.h lvm.h 
lstrlib.o : lstrlib.c lprefix.h lua.h luaconf.h lauxlib.h lualib.h 
lvm.o : lvm.c lprefix.h lua.h luaconf.h ldebug.h lstate.h lobject.h \
  llimits.h ltm.h lzio.h lmem.h ldo.h lfunc.h lgc.h lopcodes.h lstring.h \
  ltable.h lvm.h 
