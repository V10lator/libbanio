#pragma once

#define RDRAM_TO_PTR(rdram, type, var) ((type*)(&rdram[(uint32_t)var - 0x80000000]))
#define TO_PTR(type, var) RDRAM_TO_PTR(rdram, type, var);
#define PTR(x) int32_t

#define RECOMP_RETURN(x) ctx->r2 = (int32_t)x; return

#if defined(_WIN32)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __attribute__((visibility("default")))
#endif

typedef struct {
    uint64_t r0,  r1,  r2,  r3,  r4,  r5,  r6,  r7,
    r8,  r9,  r10, r11, r12, r13, r14, r15,
    r16, r17, r18, r19, r20, r21, r22, r23,
    r24, r25, r26, r27, r28, r29, r30, r31;
    uint64_t f0,  f1,  f2,  f3,  f4,  f5,  f6,  f7,
    f8,  f9,  f10, f11, f12, f13, f14, f15,
    f16, f17, f18, f19, f20, f21, f22, f23,
    f24, f25, f26, f27, f28, f29, f30, f31;
    uint64_t hi, lo;
    uint32_t* f_odd;
    uint32_t status_reg;
    uint8_t mips3_float_mode;
} recomp_context;

#if defined(_WIN32)
#include <stdlib.h>
#define bswap_32 _byteswap_ulong
#else
#include <byteswap.h>
#endif
