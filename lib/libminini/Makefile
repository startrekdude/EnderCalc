DEBUG=
DEBUG=-D NDEBUG

CC=gcc
CFLAGS=-pie -std=c99 -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -O2 $(DEBUG)

.DEFAULT_GOAL := libminini.a

ALL_OBJECTS=build/minIni.o

DIR_GUARD=@if not exist build mkdir build

build/minIni.o : minIni.c minIni.h minGlue.h

$(ALL_OBJECTS) :
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -o $@ -c $<

libminini.a : $(ALL_OBJECTS)
	ar rcu $@ $^
	ranlib $@

.PHONY : clean
clean :
	rd /q /s build
