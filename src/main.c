#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <stdint.h>

void call(void* fptr, char *fstring, ...)
{
    va_list args;
    va_start(args, fstring);
    
    uint64_t *iargs = calloc(6, sizeof(uint64_t));
    double *dargs = calloc(8, sizeof(double));

    size_t siargs_len = 100;
    size_t sdargs_len = 100;
    size_t sldargs_len = 100;
    uint64_t *siargs = malloc(siargs_len * sizeof(uint64_t));
    long double *sdargs = malloc(sdargs_len * sizeof(long double));
    long double *sldargs = malloc(sldargs_len * sizeof(long double));

    size_t ii = 0, di = 0,
           sii = 0, sdi = 0, sldi = 0;

    for (char* c = strchr(fstring, '%'); c != NULL; c = strchr(++c, '%')) {
        switch (*++c) {
            case 'i':
                if (strncmp(++c, "32", 2) == 0) {
                    int arg = va_arg(args, uint32_t);
                    if (ii < 6) {
                        iargs[ii++] = (uint64_t)arg;
                    } else {
                        if (sii >= siargs_len) {
                            siargs_len += 100;
                            siargs = realloc(siargs, siargs_len * sizeof(uint64_t));
                        }
                        siargs[sii++] = (uint64_t)arg;
                    }
                } else if (strncmp(c, "64", 2) == 0) {
                    long long int arg = va_arg(args, uint64_t);
                    if (ii < 6) {
                        iargs[ii++] = arg;
                    } else {
                        if (sii >= siargs_len) {
                            siargs_len += 100; siargs = realloc(siargs, siargs_len * sizeof(uint64_t));
                        }
                        siargs[sii++] = arg;
                    }

                } else if (strncmp(c, "128", 3) == 0) {
                    __uint128_t arg = va_arg(args, __uint128_t);
                    if (ii < 5) {
                        iargs[ii++] = (uint64_t)(arg >> 64);
                        iargs[ii++] = (uint64_t)(arg);
                    } else {
                        if (sii >= siargs_len - 1) {
                            siargs_len += 100;
                            siargs = realloc(siargs, siargs_len * sizeof(uint64_t));
                        }
                        siargs[sii++] = (uint64_t)(arg >> 64);
                        siargs[sii++] = (uint64_t)(arg);
                    }
                }
                break;
            case 'f':
                if (strncmp(++c, "64", 2) == 0) {
                    double arg = va_arg(args, double);
                    if (di < 8) {
                        dargs[di++] = arg;
                    } else {
                        if (sdi >= sdargs_len) {
                            sdargs_len += 100;
                            sdargs = realloc(sdargs, sdargs_len * sizeof(uint64_t));
                        }
                        sdargs[sdi++] = arg;
                    }

                }
                else if (strncmp(c, "128", 3) == 0) {
                    long double arg = va_arg(args, __int128_t);
                    if (sldi >= sldargs_len - 1) {
                        sldargs_len += 100;
                        sldargs = realloc(sldargs, sldargs_len * sizeof(uint64_t));
                    }
                    sldargs[sldi++] = arg;
                }
                break;
            default:
                break;
        }
    }

    va_end(args);

    size_t len = (sii + sdi) * 8;
    if ((sii + sdi) % 2) {
        len += 8;
        asm volatile("lea -8(%rsp), %rsp");
    }

    for (; sii > 0; --sii)
        asm volatile("push %0;" : : "m"(siargs[sii - 1]));

    for (; sdi > 0; --sdi)
        asm volatile("push %0;" : : "m"(sdargs[sdi - 1]));

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
    free(siargs);
    free(sdargs);
    free(sldargs);
}


int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <library_path>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];

    void *handle = dlopen(path, RTLD_LAZY);
    if (!handle) { 
        printf("Error: %s\n", dlerror());
        return 1;
    }

    void* func = dlsym(handle, "test5");

    if (!func) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }
    
    call(func, 
        "%f64 %f64 %f64 %f64 %f64 %f64 %f64 %f64 %f64",
        0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9);

    dlclose(handle);

    return 0;
}
