INC_PATHS=../include

mpp: mcc_core.so

mcc_core.so: mcc_get.c mcc_mem.c mcc_vint.c mcc_vflt.c

mcc_vflt.c: mcc/core/vflt.h
mcc_vint.c: mcc/core/vint.h
mcc_get.c: mcc/core/get.h
mcc_mem.c: mcc/core/mem.h
mcc/core/vflt.h: mcc/core/vint.h
mcc/core/vint.h: mcc/core/get.h
mcc/core/get.h: mcc/core/mem.h
mcc/core/mem.h: mcc/malloc.h
mcc/malloc.h: mcc/vmalloc.h
mcc/vmalloc.h: mcc/inttypes.h
mcc/inttypes.h: mcc/stdint.h
mcc/stdint.h: mcc/limits.h
mcc/limits.h: mcc/core/sys.h
mcc/core/sys.h
