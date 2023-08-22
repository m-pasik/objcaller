#include "call.h"
#include <stdint.h>

void call(void* fptr, char *fstring, ...)
{
    va_list args;
    va_start(args, fstring);
    
    uint64_t *iargs = calloc(6, sizeof(uint64_t));
    double *dargs = calloc(8, sizeof(double));

    size_t sargs_len = 16;
    uint64_t *sargs = malloc(sargs_len * sizeof(uint64_t));

    size_t ii = 0, di = 0, si = 0;

    for (char* c = strchr(fstring, '%'); c != NULL; c = strchr(++c, '%')) {
        switch (*++c) {
            case 'i':
                if (strncmp(++c, "32", 2) == 0) {
                    int arg = va_arg(args, uint32_t);
                    if (ii < 6) {
                        iargs[ii++] = (uint64_t)arg;
                    } else {
                        if (si >= sargs_len) {
                            sargs_len *= 2;
                            sargs = realloc(sargs, sargs_len * sizeof(uint64_t));
                        }
                        sargs[si++] = (uint64_t)arg;
                    }
                } else if (strncmp(c, "64", 2) == 0) {
                    long long int arg = va_arg(args, uint64_t);
                    if (ii < 6) {
                        iargs[ii++] = arg;
                    } else {
                        if (si >= sargs_len) {
                            sargs_len *= 2;
                            sargs = realloc(sargs, sargs_len * sizeof(uint64_t));
                        }
                        sargs[si++] = arg;
                    }

                } else if (strncmp(c, "128", 3) == 0) {
                    __uint128_t arg = va_arg(args, __uint128_t);
                    if (ii < 5) {
                        iargs[ii++] = (uint64_t)(arg);
                        iargs[ii++] = (uint64_t)(arg >> 64);
                    } else {
                        if (si >= sargs_len - 1) {
                            sargs_len *= 2;
                            sargs = realloc(sargs, sargs_len * sizeof(uint64_t));
                        }
                        sargs[si++] = (uint64_t)(arg);
                        sargs[si++] = (uint64_t)(arg >> 64);

                    }
                }
                break;
            case 'f':
                if (strncmp(++c, "64", 2) == 0) {
                    double arg = va_arg(args, double);
                    if (di < 8) {
                        dargs[di++] = arg;
                    } else {
                        if (si >= sargs_len) {
                            sargs_len *= 2;
                            sargs = realloc(sargs, sargs_len * sizeof(uint64_t));
                        }
                        memcpy(sargs + si++, &arg, sizeof(double));
                    }

                }
                else if (strncmp(c, "128", 3) == 0) {
                    long double arg = va_arg(args, long double);
                    if (si >= sargs_len - 1) {
                        sargs_len *= 2;
                        sargs = realloc(sargs, sargs_len * sizeof(uint64_t));
                    }
                    memcpy(sargs + si, &arg, sizeof(long double));
                    si += 2;
                }
                break;
            default:
                break;
        }
    }

    va_end(args);

    size_t len = si * 8;
    if (si % 2) {
        len += 8;
        asm volatile("lea -8(%rsp), %rsp");
    }

    for (; si > 0; --si)
        asm volatile("push %0;" : : "m"(sargs[si - 1]));

    asm volatile (
        "movq (%0), %%xmm0\n"
        "movq 8(%0), %%xmm1\n"
        "movq 16(%0), %%xmm2\n"
        "movq 24(%0), %%xmm3\n"
        "movq 32(%0), %%xmm4\n"
        "movq 40(%0), %%xmm5\n"
        "movq 48(%0), %%xmm6\n"
        "movq 56(%0), %%xmm7\n"
        :
        : "r"(dargs)
    );
    
    //asm volatile(
    //    "movdqu (%0), %%xmm0;"
    //    "movdqu 16(%0), %%xmm1;"
    //    "movdqu 32(%0), %%xmm2;"
    //    "movdqu 48(%0), %%xmm3;"
    //    "movdqu 64(%0), %%xmm4;"
    //    "movdqu 80(%0), %%xmm5;"
    //    "movdqu 96(%0), %%xmm6;"
    //    "movdqu 112(%0), %%xmm7;"
    //    :
    //    : "r"(dargs)
    //);

    asm volatile(
        "movq (%0), %%rdi;"
        "movq 8(%0), %%rsi;"
        "movq 16(%0), %%rdx;"
        "movq 24(%0), %%rcx;"
        "movq 32(%0), %%r8;"
        "movq 40(%0), %%r9;"
        :
        : "r"(iargs)
        : "rdi", "rsi", "rdx", "rcx", "r8", "r9"
    );

    asm volatile("call *%0;"
                 "add %1, %%rsp;"
                 :
                 : "m"(fptr), "m"(len)
                 : "rdi", "rsi", "rdx", "rcx", "r8", "r9");

    free(iargs);
    free(dargs);
    free(sargs);
}
