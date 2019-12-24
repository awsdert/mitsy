LIB_PATHS=../lib
INC_PATHS=../include
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
mcc_core_sources:=$(wildcard mcc/core/*.c)
mcc_core_objects:=$(mcc_core_sources:%.c=%.o)
mcc_test_sources:=test/unit_tests.c
mcc_test_objects:=$(mcc_test_sources:%.c=%.o)
mcc_core_lib_name=libmcc_core.$(DLL)
mcc_core_lib:=../lib/$(mcc_core_lib_name)
mcc_test_app:=mcc_unit_test.$(EXE)

mcc: $(mcc_core_lib)
test: $(mcc_test_app) force
	./$(mcc_test_app)
clean:
	$(CMD_DEL) $(mcc_core_objects)
	$(CMD_DEL) $(mcc_test_objects)
	$(CMD_DEL) $(mcc_test_app) $(mcc_core_lib) $(mcc_core_lib_name)
rebuild: clean mcc

$(mcc_core_lib): $(mcc_core_objects)
	$(CC) -shared $(BFLAGS) -lpthread -o $@ $^
	$(CC) -shared $(BFLAGS) -lpthread -o $(mcc_core_lib_name) $^

$(mcc_test_app): $(mcc_test_objects) $(mcc_core_lib)
	$(CC) $(BFLAGS) -lmcc_core -o $@ $(mcc_test_objects)
	
%.o: %.c
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
mcc/inttypes.h: mcc/stdint.h
mcc/stdint.h: mcc/limits.h
mcc/limits.h: mcc/core/sys.h
mcc/core/sys.h:
test/unit_tests.c: test/core/vint.c

force: ;
