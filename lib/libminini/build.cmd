@echo off

set DEBUG=
set DEBUG="-D NDEBUG"

mkdir build

gcc -pie -std=c99 -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -O2 %DEBUG% -o build/minIni.o -c minIni.c

ar rcu libminini.a build/minIni.o
ranlib libminini.a

rd /q /s build