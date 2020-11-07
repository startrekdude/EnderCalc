DEBUG=
DEBUG=-D NDEBUG

CC=gcc
CFLAGS=-pie -std=c99 -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -O2 $(DEBUG)

.DEFAULT_GOAL := libbf.a

ALL_OBJECTS=build/cutils.o build/libbf.o

ifeq ($(OS),Windows_NT)
	DIR_GUARD=@if not exist build mkdir build
	PLATFORM_RMDIR=rd /q /s
else
	DIR_GUARD=@mkdir -p build
	PLATFORM_RMDIR=rm -rf
endif

build/cutils.o : cutils.c cutils.h
build/libbf.o : libbf.c libbf.h cutils.h

$(ALL_OBJECTS) :
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -o $@ -c $<

libbf.a : $(ALL_OBJECTS)
	ar rcu $@ $^
	ranlib $@

.PHONY : clean
clean :
	$(PLATFORM_RMDIR) build
