#include "../include/call.h"
#include <stdint.h>
#include <string.h>

void call(void* fptr, char *fstring, void *args)
{
    uint8_t *data = (uint8_t*)args;

    /* Arrays of integer and float arguments to be passed in registers */
    uint64_t *iargs = calloc(6, sizeof(uint64_t));
    double *fargs = calloc(8, sizeof(double));
    
    /* List of arguments to be passed in stack */
    size_t sargs_len = 16;
    uint64_t *sargs = malloc(sargs_len * sizeof(uint64_t));

    size_t ii = 0, fi = 0, si = 0; /* indexes of iargs, dargs, sargs */

    /*
     * Read varargs based on format string.
     */

    for (char* c = strchr(fstring, '%'); c != NULL; c = strchr(++c, '%')) {
        switch (*++c) {
            case 'i':   /* Integer */
                if (strncmp(++c, "32", 2) == 0) {   /* 32-bit */
                    uint32_t arg;
                    memcpy(&arg, data, sizeof(uint32_t));
                    data += sizeof(uint32_t);

                    if (ii < 6) {
                        iargs[ii++] = (uint64_t)arg;
                    } else {
                        if (si >= sargs_len) {
                            sargs_len *= 2;
                            sargs = realloc(sargs, sargs_len * sizeof(uint64_t));
                        }
                        sargs[si++] = (uint64_t)arg;
                    }
                } else if (strncmp(c, "64", 2) == 0) {  /* 64-bit */
                    uint64_t arg;
                    memcpy(&arg, data, sizeof(uint64_t));
                    data += sizeof(uint64_t);

                    if (ii < 6) {
                        iargs[ii++] = arg;
                    } else {
                        if (si >= sargs_len) {
                            sargs_len *= 2;
                            sargs = realloc(sargs, sargs_len * sizeof(uint64_t));
                        }
                        sargs[si++] = arg;
                    }

                } else if (strncmp(c, "128", 3) == 0) { /* 128-bit */
                    __uint128_t arg;
                    memcpy(&arg, data, sizeof(__uint128_t));
                    data += sizeof(__uint128_t);

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
                /* 
                 * Float
                 */
                if (strncmp(++c, "64", 2) == 0) {  /* 64-bit */
                    double arg;
                    memcpy(&arg, data, sizeof(double));
                    data += sizeof(double);

                    if (fi < 8) {
                        fargs[fi++] = arg;
                    } else {
                        if (si >= sargs_len) {
                            sargs_len *= 2;
                            sargs = realloc(sargs, sargs_len * sizeof(uint64_t));
                        }
                        memcpy(sargs + si++, &arg, sizeof(double));
                    }

                }
                else if (strncmp(c, "128", 3) == 0) {   /* 128-bit */
                    long double arg;
                    memcpy(&arg, data, sizeof(long double));
                    data += sizeof(long double);

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

    /*
     * Passing arguments and calling function
     */

    asm volatile(
        /* Set r12 to si * 8. */ 
        "movq %[si], %%r12;"
        "shlq $3, %%r12;"

        /* If si is odd add 8 byte offset to the stack and add 8 to r12. */
        "movq %[si], %%rax;"
        "and $1, %%rax;"
        "cmp $0, %%rax;"
        "je cmp;"
        "lea -8(%%rsp), %%rsp;"
        "addq $8, %%r12;"
        
        /* Push all arguments from sargs to the stack. */
        "jmp cmp;"
        "loop:"
        "push (%[sargs]);"
        "lea -8(%[sargs]), %[sargs];"
        "subq $1, %[si];"
        "cmp:"
        "cmp $0, %[si];"
        "jne loop;"

        /* Move floating-point arguments to xmm registers. */
        "movq (%[fargs]), %%xmm0;"
        "movq 8(%[fargs]), %%xmm1;"
        "movq 16(%[fargs]), %%xmm2;"
        "movq 24(%[fargs]), %%xmm3;"
        "movq 32(%[fargs]), %%xmm4;"
        "movq 40(%[fargs]), %%xmm5;"
        "movq 48(%[fargs]), %%xmm6;"
        "movq 56(%[fargs]), %%xmm7;"
        
        /* Move integer arguments to integer registers. */
        "movq (%[iargs]), %%rdi;"
        "movq 8(%[iargs]), %%rsi;"
        "movq 16(%[iargs]), %%rdx;"
        "movq 24(%[iargs]), %%rcx;"
        "movq 32(%[iargs]), %%r8;"
        "movq 40(%[iargs]), %%r9;"

        /* Call the function. */
        "call *%[fptr];"

        /* Bring stack back to its original state. */
        "add %%r12, %%rsp;"
        : 
        : [si] "r"(si), [sargs] "r"(&sargs[si-1]),
          [fargs] "r"(fargs), [iargs] "r"(iargs),
          [fptr] "m"(fptr)
        : "rax", "rdi", "rsi", "rdx", "rcx", "r8", "r9", "r12"
    );

    free(iargs);
    free(fargs);
    free(sargs);
}
