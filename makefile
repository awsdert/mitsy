DEF?=
EXE_EXT?=elf
DLL_EXT?=so
LIB_EXT?=a
PFX?=
EXT?=${EXE_EXT}
CFLAGS=-fPIC -Wall -Wno-multichar
SRC=$(wildcard *.c)
CPP=$(wildcard *.cpp)
INC=$(wildcard *.h)
HPP=$(wildcard *.hpp)
EXE_TARGETS=main word unit_tests
DLL_TARGETS=mcc_mem mcc_get mcc_tsc mcc_base62
TARGETS=${EXE_TARGETS} ${DLL_TARGETS}

all: buildall
runall: ${EXE_TARGETS:%=%.run}
rebuildall: cleanall buildall
buildall: alldebug allplain
allplain: ${EXE_TARGETS:%=%.plainexe} ${DLL_TARGETS:%=%.plaindll}
alldebug: ${EXE_TARGETS:%=%.debugexe} ${DLL_TARGETS:%=%.debugdll}
cleanall: ${EXE_TARGETS:%=%.cleanexe} ${DLL_TARGETS:%=%.cleandll}

.PHONY: all runall rebuildall buildall cleanall

%.rebuildexe: %.buildexe %.cleanexe next.cleandll
	#$@, message is to trick make into seeing the target

%.rebuilddll: %.builddll %.cleandll
	#$@, message is to trick make into seeing the target

%.rebuildlib: %.buildlib %.cleanlib
	#$@, message is to trick make into seeing the target

%.run: %.${EXE_EXT}
	./$<

%.dbg: d%.${EXE_EXT}
	gdb ./$<

%.plainexe: %.${EXE_EXT}
	#$@, message is to trick make into seeing the target

%.plaindll: lib%.${DLL_EXT}
	#$@, message is to trick make into seeing the target

%.plainlib: lib%.${LIB_EXT}
	#$@, message is to trick make into seeing the target

%.debugexe: d%.${EXE_EXT}
	#$@, message is to trick make into seeing the target

%.debugdll: libd%.${DLL_EXT}
	#$@, message is to trick make into seeing the target

%.debuglib: libd%.${LIB_EXT}
	#$@, message is to trick make into seeing the target

%.cleanexe: %.${EXE_EXT}
	rm -f $<

%.cleandll: lib%.${DLL_EXT}
	rm -f $<

%.cleanlib: lib%.${LIB_EXT}
	rm -f $<

%.exe: %.c ${DLL_TARGETS:%=lib%.dll}
	cc ${CFLAGS} ${DEF} -D OUT=$@ -o ./$@ $< ${DLL_TARGETS:%=./lib%.dll}

%64.exe: %.c ${DLL_TARGETS:%=lib%64.dll}
	cc ${CFLAGS} ${DEF} -D OUT=$@ -o ./$@ $< ${DLL_TARGETS:%=./lib%64.dll}

%.elf: %.c ${DLL_TARGETS:%=lib%.so}
	cc ${CFLAGS} ${DEF} -D OUT=$@ -o ./$@ $< ${DLL_TARGETS:%=./lib%.so}

d%.exe: %.c ${DLL_TARGETS:%=libd%.dll}
	cc -ggdb ${CFLAGS} ${DEF} -D OUT=$@ -o ./$@ $< ${DLL_TARGETS:%=./libd%.dll}

d%64.exe: %.c ${DLL_TARGETS:%=libd%64.dll}
	cc -ggdb ${CFLAGS} ${DEF} -D OUT=$@ -o ./$@ $< ${DLL_TARGETS:%=./libd%64.dll}

d%.elf: %.c ${DLL_TARGETS:%=libd%.so}
	cc -ggdb ${CFLAGS} ${DEF} -D OUT=$@ -o ./$@ $< ${DLL_TARGETS:%=./libd%.so}

lib%.dll: %.c
	cc ${CFLAGS} ${DEF} -shared -o ./$@ -c $<

lib%64.dll: %.c
	cc ${CFLAGS} ${DEF} -shared -o ./$@ -c $<

lib%.so: %.c
	cc ${CFLAGS} ${DEF} -shared -o ./$@ -c $<

lib%.a: %.c
	cc ${CFLAGS} ${DEF} -o ./$@ -c $<

libd%.dll: %.c
	cc -ggdb ${CFLAGS} ${DEF} -shared -o ./$@ -c $<

libd%64.dll: %.c
	cc -ggdb ${CFLAGS} ${DEF} -shared -o ./$@ -c $<

libd%.so: %.c
	cc -ggdb ${CFLAGS} ${DEF} -shared -o ./$@ -c $<

libd%.a: %.c
	cc -ggdb ${CFLAGS} ${DEF} -o ./$@ -c $<

mcc_%.c: mcc_%.h
mcc_get.h: mcc_err.h mcc_bitsof.h mcc_mem.h
mcc_tsc.h: mcc_err.h mcc_bitsof.h
mcc_base62.h: mcc_err.h mcc_bitsof.h
mcc_mem.h: mcc_err.h
