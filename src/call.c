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
    if (si % 2)
        len += 8;

    asm volatile(
        "movq %[si], %%rax;"
        "and $1, %%rax;"
        "cmp $0, %%rax;"
        "je cmp;"
        "lea -8(%%rsp), %%rsp;"
        
        "jmp cmp;"
        "loop:"
        "push (%[sargs]);"
        "lea -8(%[sargs]), %[sargs];"
        "subq $1, %[si];"
        "cmp:"
        "cmp $0, %[si];"
        "jne loop;"

        "movq (%[dargs]), %%xmm0;"
        "movq 8(%[dargs]), %%xmm1;"
        "movq 16(%[dargs]), %%xmm2;"
        "movq 24(%[dargs]), %%xmm3;"
        "movq 32(%[dargs]), %%xmm4;"
        "movq 40(%[dargs]), %%xmm5;"
        "movq 48(%[dargs]), %%xmm6;"
        "movq 56(%[dargs]), %%xmm7;"
        
        "movq (%[iargs]), %%rdi;"
        "movq 8(%[iargs]), %%rsi;"
        "movq 16(%[iargs]), %%rdx;"
        "movq 24(%[iargs]), %%rcx;"
        "movq 32(%[iargs]), %%r8;"
        "movq 40(%[iargs]), %%r9;"

        "call *%[fptr];"
        "add %[len], %%rsp;"
        : 
        : [si] "r"(si), [sargs] "r"(&sargs[si-1]),
          [dargs] "r"(dargs), [iargs] "r"(iargs),
          [fptr] "m"(fptr), [len] "m"(len)
        : "rax", "rdi", "rsi", "rdx", "rcx", "r8", "r9", "cc", "memory"
    );

    free(iargs);
    free(dargs);
    free(sargs);
}
