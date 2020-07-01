@echo off

set DEBUG=
set DEBUG="-D NDEBUG"

mkdir build

gcc -pie -std=c99 -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -O2 %DEBUG% -I . -o build/history.o -c history.c
gcc -pie -std=c99 -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -O2 %DEBUG% -I . -o build/fn_complete.o -c fn_complete.c
gcc -pie -std=c99 -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -O2 %DEBUG% -I . -o build/editline.o -c editline.c

ar rcu libedit.a build/history.o build/fn_complete.o build/editline.o
ranlib libedit.a

rd /q /s build