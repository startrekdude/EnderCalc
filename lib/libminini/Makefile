DEBUG=
DEBUG=-D NDEBUG

CC=gcc
CFLAGS=-pie -std=c99 -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -O2 $(DEBUG)

.DEFAULT_GOAL := libminini.a

ALL_OBJECTS=build/minIni.o

ifeq ($(OS),Windows_NT)
	DIR_GUARD=@if not exist build mkdir build
	PLATFORM_RMDIR=rd /q /s
else
	DIR_GUARD=@mkdir -p build
	PLATFORM_RMDIR=rm -rf
endif

build/minIni.o : minIni.c minIni.h minGlue.h

$(ALL_OBJECTS) :
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -o $@ -c $<

libminini.a : $(ALL_OBJECTS)
	ar rcu $@ $^
	ranlib $@

.PHONY : clean
clean :
	$(PLATFORM_RMDIR) build
