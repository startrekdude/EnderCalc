@echo off

set DEBUG=
set DEBUG="-D NDEBUG"

mkdir build

gcc -pie -std=c99 -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -O2 %DEBUG% -o build/cutils.o -c cutils.c
gcc -pie -std=c99 -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -O2 %DEBUG% -o build/libbf.o -c libbf.c

ar rcu libbf.a build/cutils.o build/libbf.o
ranlib libbf.a

rd /q /s build