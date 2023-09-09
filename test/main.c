#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/loader.h"
#include "../include/call.h"
#include "test.h"

int main(int argc, char **argv)
{
    void *handle = load_object("./libtesting.so");
    if (!handle)
        return 1;

    void *func = NULL;

    if (strcmp(argv[1], "test1") == 0) {
        func = load_function(handle, "test1");

        if (!func) {
            close_object(handle);
            return 1;
        }

        int32_t data[] = {1, 2, 3, 4, 5, 6};

        call(func, "%i32 %i32 %i32 %i32 %i32 %i32", &data);

    } else if (strcmp(argv[1], "test2") == 0) {
        func = load_function(handle, "test2");

        if (!func) {
            close_object(handle);
            return 1;
        }
    
        int64_t data[] = {1, 2, 3, 4, 5, 6};

        call(func, "%i64 %i64 %i64 %i64 %i64 %i64", &data);

    } else if (strcmp(argv[1], "test3") == 0) {
        func = load_function(handle, "test3");

        if (!func) {
            close_object(handle);
            return 1;
        }

        __int128_t data[] = {1, 2, 3};

        call(func, "%i128 %i128 %i128", &data);

    } else if (strcmp(argv[1], "test4") == 0) {
        func = load_function(handle, "test4");

        if (!func) {
            close_object(handle);
            return 1;
        }

        int32_t data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        call(func, "%i32 %i32 %i32 %i32 %i32 %i32 %i32 %i32 %i32 %i32", &data);

    } else if (strcmp(argv[1], "test5") == 0) {
        func = load_function(handle, "test5");

        if (!func) {
            close_object(handle);
            return 1;
        }

        int64_t data[] = {1, 2, 0, 3, 0, 4, 0, 5};

        call(func, "%i64 %i128 %i128 %i128 %i64", &data);

    } else if (strcmp(argv[1], "test6") == 0) {
        func = load_function(handle, "test6");

        if (!func) {
            close_object(handle);
            return 1;
        }

        double data[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};

        call(func, "%f64 %f64 %f64 %f64 %f64 %f64 %f64 %f64", data);

    } else if (strcmp(argv[1], "test7") == 0) {
        func = load_function(handle, "test7");

        if (!func) {
            close_object(handle);
            return 1;
        }

        double data[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

        call(func, "%f64 %f64 %f64 %f64 %f64 %f64 %f64 %f64 %f64 %f64", data);

    } else if (strcmp(argv[1], "test8") == 0) {
        func = load_function(handle, "test8");

        if (!func) {
            close_object(handle);
            return 1;
        }

        long double data[] = {0.1, 0.2, 0.3};

        call(func, "%f128 %f128 %f128", data);

    }  else if (strcmp(argv[1], "test9") == 0) {
        func = load_function(handle, "test9");

        if (!func) {
            close_object(handle);
            return 1;
        }

        double fdata[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
        uint32_t idata[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        uint8_t data[sizeof(fdata) + sizeof(idata)];

        double *fdatap = fdata;
        uint32_t *idatap = idata;
        uint8_t *datap = data;

        for (size_t i = 0; i < 19; i++) {
            if (i % 2) {
                *(uint32_t*)datap = *idatap++;
                datap += 4;
            } else {
                *(double*)datap = *fdatap++;
                datap += 8;
            }
        }
        
        call(func,
             "%f64 %i32 %f64 %i32 %f64 %i32 %f64 %i32 %f64 %i32 %f64 %i32 %f64 %i32 %f64 %i32 %f64 %i32 %f64",
             data);
    }

    return 0;
}
