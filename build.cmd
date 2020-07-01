@echo off

set DEBUG=
set DEBUG="-D NDEBUG"

mkdir build

gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/vector.o -c src/vector.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/expr.o -c src/expr.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/symbols.o -c src/symbols.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/parse.o -c src/parse.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/parse_match_brackets.o -c src/parse_match_brackets.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/parse_tokenize.o -c src/parse_tokenize.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/parse_ezpass.o -c src/parse_ezpass.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/mem.o -c src/mem.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/eval.o -c src/eval.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/engine.o -c src/engine.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/engine_basic.o -c src/engine/engine_basic.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/engine_trig.o -c src/engine/engine_trig.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/engine_hyperbolic.o -c src/engine/engine_hyperbolic.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/engine_stats.o -c src/engine/engine_stats.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -D HELP_USE_WIN32 -o build/command.o -c src/command.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/stats.o -c src/stats.c
gcc -pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include %DEBUG% -o build/calc.o -c src/calc.c

windres rsrc/version.rc build/version.o
windres rsrc/icon.rc build/icon.o

gcc -pie -Wl,-emainCRTStartup -Wl,--dynamicbase -Wl,--gc-sections -Wl,-s -std=c99 -flto -O2 -o EnderCalc.exe^
 build/vector.o build/expr.o  build/symbols.o build/parse.o build/parse_match_brackets.o^
 build/parse_tokenize.o build/parse_ezpass.o build/mem.o build/eval.o build/engine.o^
 build/engine_basic.o build/engine_trig.o build/engine_hyperbolic.o build/engine_stats.o^
 build/version.o build/command.o build/stats.o build/calc.o build/icon.o^
 lib/libbf/libbf.a lib/libedit/libedit.a lib/libminini/libminini.a -lshlwapi

rd /q /s build