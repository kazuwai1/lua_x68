# lua_x68
This is Lua 5.4.6 for X68000(Japanese old computer).


[build environment]

cc: gcc.x

as: has.x

lk: hlk.x

lib: XC2.0 library

make: gnu make


[build]

Please type "gmake -f Makefile.x68"  in "src" directory when you want to build.


[install]

After building:

copy lua.x and luac.x to the any directory included in "%PATH%".

copy lualib.l to "%LIB%" directory.

copy below header files to "%INCLUDE%" directory.

 lauxlib.h
 
 lua.h
 
 lua.hpp
 
 luaconf.h
 
 lualib.h
 

[Tips for first time use]

To exit from lua, use "os.exit()" or type ctrl-z.
