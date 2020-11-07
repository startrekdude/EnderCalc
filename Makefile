DEBUG=
DEBUG=-D NDEBUG

CC=gcc
CFLAGS=-pie -std=c99 -flto -fno-asynchronous-unwind-tables -O2 -I include $(DEBUG) -D HELP_USE_WIN32
.DEFAULT_GOAL := EnderCalc.exe

LDFLAGS=-pie -Wl,-emainCRTStartup -Wl,--dynamicbase -Wl,--gc-sections -Wl,-s -std=c99 -flto -O2
LDLIBS=-lshlwapi

I_VECTOR_H=include/vector.h
I_SYMBOLS_H=include/symbols.h
I_LIBBF_H=include/libbf.h
I_CALC_H=include/calc.h
I_COMMAND_H=include/command.h
I_MINGLUE_H=include/minGlue.h
I_STATS_H=include/stats.h
I_READLINE_H=include/readline.h
I_MININI_H=include/minini.h $(I_MINGLUE_H)
I_EXPR_H=include/expr.h $(I_VECTOR_H)
I_ENGINE_H=include/engine.h $(I_LIBBF_H)
I_PARSE_H=include/parse.h $(I_VECTOR_H)
I_MEM_H=include/mem.h $(I_LIBBF_H)
I_EVAL_H=include/eval.h $(I_LIBBF_H) $(I_VECTOR_H)

ALL_OBJECTS=build/vector.o build/expr.o build/symbols.o build/parse.o build/parse_match_brackets.o\
  build/parse_tokenize.o build/parse_ezpass.o build/mem.o build/eval.o build/engine.o build/engine_basic.o\
  build/engine_trig.o build/engine_hyperbolic.o build/engine_stats.o build/command.o build/stats.o build/calc.o

ALL_RESOURCES=build/version.o build/icon.o

ALL_LIBRARIES=lib/libbf/libbf.a lib/libedit/libedit.a lib/libminini/libminini.a

build :
	mkdir build

build/vector.o : src/vector.c $(I_VECTOR_H) build
build/expr.o : src/expr.c $(I_EXPR_H) $(I_ENGINE_H) $(I_SYMBOLS_H) $(I_LIBBF_H) build
build/symbols.o : src/symbols.c $(I_SYMBOLS_H) $(I_ENGINE_H) build
build/parse.o : src/parse.c $(I_PARSE_H) $(I_EXPR_H) build
build/parse_match_brackets.o : src/parse_match_brackets.c $(I_PARSE_H) $(I_EXPR_H) build
build/parse_tokenize.o : src/parse_tokenize.c $(I_PARSE_H) $(I_ENGINE_H) $(I_EXPR_H) $(I_SYMBOLS_H) $(I_LIBBF_H) build
build/parse_ezpass.o : src/parse_ezpass.c $(I_PARSE_H) $(I_ENGINE_H) $(I_EXPR_H) $(I_SYMBOLS_H) build
build/mem.o : src/mem.c $(I_MEM_H) $(I_CALC_H) $(I_COMMAND_H) $(I_ENGINE_H) $(I_EVAL_H) $(I_MININI_H) build
build/eval.o : src/eval.c $(I_EVAL_H) $(I_ENGINE_H) $(I_EXPR_H) $(I_MEM_H) $(I_SYMBOLS_H) build
build/engine.o : src/engine.c $(I_ENGINE_H) $(I_EVAL_H) build
build/engine_basic.o : src/engine/engine_basic.c $(I_ENGINE_H) $(I_EVAL_H) build
build/engine_trig.o : src/engine/engine_trig.c $(I_ENGINE_H) $(I_EVAL_H) build
build/engine_hyperbolic.o : src/engine/engine_hyperbolic.c $(I_ENGINE_H) $(I_EVAL_H) build
build/engine_stats.o : src/engine/engine_stats.c $(I_ENGINE_H) $(I_EVAL_H) build
build/command.o : src/command.c $(I_COMMAND_H) $(I_CALC_H) $(I_ENGINE_H) $(I_EVAL_H) $(I_MEM_H) $(I_PARSE_H)\
  $(I_STATS_H) $(I_MININI_H) build
build/stats.o : src/stats.c $(I_STATS_H) $(I_CALC_H) $(I_ENGINE_H) $(I_VECTOR_H) $(I_LIBBF_H) $(I_READLINE_H) build
build/calc.o : src/calc.c $(I_CALC_H) $(I_COMMAND_H) $(I_ENGINE_H) $(I_EVAL_H) $(I_EXPR_H) $(I_MEM_H)\
 $(I_PARSE_H) $(I_READLINE_H) build

$(ALL_OBJECTS) :
	$(CC) $(CFLAGS) -o $@ -c $<

build/version.o : rsrc/version.rc
build/icon.o : rsrc/icon.rc rsrc/EnderCalc.ico

$(ALL_RESOURCES) :
	windres $< $@

EnderCalc.exe : $(ALL_OBJECTS) $(ALL_RESOURCES) $(ALL_LIBRARIES)
	$(CC) $(LDFLAGS) -o $@ $(ALL_OBJECTS) $(ALL_RESOURCES) $(ALL_LIBRARIES) $(LDLIBS)

.PHONY : clean
clean :
	rd /q /s build