DEBUG=
DEBUG=-D NDEBUG

CC=gcc
CFLAGS=-pie -std=c99 -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables -O2 $(DEBUG) -I .

.DEFAULT_GOAL := libedit.a

ALL_OBJECTS=build/history.o build/fn_complete.o build/editline.o

DIR_GUARD=@if not exist build mkdir build

build/history.o : history.c editline/readline.h editline/wineditline.h
build/fn_complete.o : fn_complete.c editline/readline.h editline/wineditline.h
build/editline.o : editline.c el_globals.h editline/readline.h editline/wineditline.h

$(ALL_OBJECTS) :
	$(DIR_GUARD)
	$(CC) $(CFLAGS) -o $@ -c $<

libedit.a : $(ALL_OBJECTS)
	ar rcu $@ $^
	ranlib $@

.PHONY : clean
clean :
	rd /q /s build
