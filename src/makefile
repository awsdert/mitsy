mcc_lib_path=../lib
mcc_inc_path=../include
LIB_PATHS:=$(mcc_lib_path)
INC_PATHS:=$(mcc_inc_path)
IS_WINDOWS:=$(if $(Program Files),1,)
IS_WIN64:=$(if ${Program Files(x86)},1,)
IS_WIN32:=$(if $(IS_WIN64),,$(IS_WINDOWS))
EXE:=$(if $(IS_WINDOWS),exe,AppImage)
DLL:=$(if $(IS_WINDOWS),dll,so)
CMD_DEL:=$(if $(IS_WINDOWS),del,rm -f)
LFLAGS:=$(LIB_PATHS:%=-L %)
BFLAGS:=-fPIC $(LFLAGS)
IFLAGS:=$(INC_PATHS:%=-I %)
CFLAGS:=-Wall -fPIC $(IFLAGS)
CC=clang
mcc_core_sources:=$(wildcard mcc/core/*.c) $(wildcard mcc/*.c)
mcc_core_objects:=$(mcc_core_sources:%=%.o)
mcc_test_sources:=test.c $(wildcard test/core/*.c)
mcc_test_objects:=$(mcc_test_sources:%=%.o)
mcc_core_lib_name=libmcc_core.$(DLL)
mcc_core_lib:=$(mcc_lib_path)/$(mcc_core_lib_name)
mcc_test_app:=test.$(EXE)
MAKECMDGOALS?=all
all: build
build: $(mcc_core_lib) $(mcc_test_app) force
mcc: $(mcc_core_lib)
test: $(mcc_test_app) force
	./$(mcc_test_app)
clean:
	$(CMD_DEL) mcc/core/*.o
	$(CMD_DEL) mcc/*.o
	$(CMD_DEL) test/core/*.o
	$(CMD_DEL) test/*.o
	$(CMD_DEL) *.o
	$(CMD_DEL) $(mcc_test_app) $(mcc_core_lib) $(mcc_core_lib_name)
rebuild: clean build force

$(mcc_core_lib): $(mcc_core_objects)
	$(CC) -shared $(BFLAGS) -lpthread -o $@ $^

$(mcc_test_app): $(mcc_test_objects) $(mcc_core_lib)
	$(CC) $(BFLAGS) -lmcc_core -o $@ $(mcc_test_objects) -Wl,-rpath,$(LIB_PATHS)
	
%.c.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

mcc/core/vflt.c: mcc/core/vflt.h
mcc/core/vint.c: mcc/core/vint.h
mcc/core/get.c: mcc/core/get.h
mcc/core/mem.c: mcc/core/mem.h
mcc/core/vflt.h: mcc/core/vint.h
mcc/core/vint.h: mcc/core/get.h
mcc/core/get.h: mcc/core/mem.h
mcc/core/mem.h: mcc/malloc.h
mcc/malloc.h: mcc/vmalloc.h
mcc/vmalloc.h: mcc/inttypes.h
mcc/time.h: mcc/bitsof.h
mcc/bitsof.h: mcc/stdint.h
mcc/inttypes.h: mcc/stdint.h
mcc/stdint.h: mcc/limits.h
mcc/limits.h: mcc/core/sys.h
mcc/core/sys.h:
test.c: test/core/vint.h
test/core/%.c: test/core/%.h
test/core/vint.h: mcc/core/vint.h mcc/time.h
test/core/vflt.h: test/core/vint.h

force: ;
.PHONY: all mcc test clean rebuild
