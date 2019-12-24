INC_PATHS=../include
IS_WINDOWS:=$(if $(Program Files),1,)
IS_WIN64:=$(if ${Program Files(x86)},1,)
IS_WIN32:=$(if $(IS_WIN64),,$(IS_WINDOWS))
EXE?=$(if $(IS_WINDOWS),exe,AppImage)
IFLAGS:=$(INC_PATHS:%=-I %)
CFLAGS:=-Wall $(IFLAGS)
mcc_core_sources:=$(wildcard mcc/core/*.c)
mcc_core_objects:=$(mcc_core_sources:%.c=%.o)
mcc_test_sources:=test/unit_tests.c
mcc_test_objects:=$(test_core_sources:%.c=%.o)

mcc: libmcc_core.so
test: test.$(EXE)

libmcc_core.so: $(mcc_test_objects)
	gcc -lpthread -o $@ $^

test.AppImage: $(mcc_test_objects) libmcc_core.so
	gcc -l ./libmcc_core.so -o $@ $(mcc_test_objects)

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
test/unit_tests.o: libmcc_core.so

%.o: %.c
	clang $(CFLAGS) -o $@ -c $<
	
test/unit_tests.c: test/core/vint.c

force: ;
