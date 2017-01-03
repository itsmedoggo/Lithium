@echo off

setlocal

set LFLAGS=--bc-target=ZDoom --bc-zdacs-init-delay --bc-zdacs-chunk-STRE
set CFLAGS=--bc-target=ZDoom -iinc -Dnull=NULL

if not exist bin/libc.ir (
   C:\gdcc\gdcc-makelib.exe --bc-target=ZDoom -c libc -o bin/libc.ir
)

if not exist bin/libGDCC.ir (
   C:\gdcc\gdcc-makelib.exe --bc-target=ZDoom -c libGDCC -o bin/libGDCC.ir
)

if not exist ../../acs/lithlib.bin (
   C:\gdcc\gdcc-ld.exe %LFLAGS% --func-minimum ScriptI 1200 --alloc-min Sta "" 300000000 ^
      bin/libGDCC.ir ^
      bin/libc.ir ^
      -o ..\..\acs\lithlib.bin
)

C:\gdcc\gdcc-cc.exe %CFLAGS% -c src/lith_list.c -o bin/lith_list.ir
C:\gdcc\gdcc-ld.exe %LFLAGS% --func-minimum ScriptI 1300 --alloc-min Sta "" 1000000000 ^
   -llithlib ^
   bin/lith_list.ir ^
   -o ..\..\acs\lithmisc.bin

C:\gdcc\gdcc-cc.exe %CFLAGS% -c src/lith_common.c -o bin/lith_common.ir
C:\gdcc\gdcc-cc.exe %CFLAGS% -c src/lith_player.c -o bin/lith_player.ir
C:\gdcc\gdcc-cc.exe %CFLAGS% -c src/lith_world.c -o bin/lith_world.ir
C:\gdcc\gdcc-cc.exe %CFLAGS% -c src/lith_hud.c -o bin/lith_hud.ir
C:\gdcc\gdcc-cc.exe %CFLAGS% -c src/lith_decorate.c -o bin/lith_decorate.ir
C:\gdcc\gdcc-cc.exe %CFLAGS% -c src/lith_pickups.c -o bin/lith_pickups.ir
C:\gdcc\gdcc-cc.exe %CFLAGS% -c src/lith_upgrades.c -o bin/lith_upgrades.ir
C:\gdcc\gdcc-cc.exe %CFLAGS% -c src/lith_cbi.c -o bin/lith_cbi.ir
C:\gdcc\gdcc-cc.exe %CFLAGS% -c src/lith_cbi_gui.c -o bin/lith_cbi_gui.ir
C:\gdcc\gdcc-ld.exe %LFLAGS% --func-minimum ScriptI 1400 --alloc-min Sta "" 3000000000 ^
   -llithlib ^
   -llithmisc ^
   bin/lith_common.ir ^
   bin/lith_player.ir ^
   bin/lith_world.ir ^
   bin/lith_hud.ir ^
   bin/lith_decorate.ir ^
   bin/lith_pickups.ir ^
   bin/lith_upgrades.ir ^
   bin/lith_cbi.ir ^
   bin/lith_cbi_gui.ir ^
   -o ..\..\acs\lithmain.bin

endlocal
pause
