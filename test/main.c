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

        call(func,
             "%i32 %i32 %i32 %i32 %i32 %i32",
             1, 2, 3, 4, 5, 6);

    } else if (strcmp(argv[1], "test2") == 0) {
        func = load_function(handle, "test2");

        if (!func) {
            close_object(handle);
            return 1;
        }

        call(func,
             "%i64 %i64 %i64 %i64 %i64 %i64",
             1LL, 2LL, 3LL, 4LL, 5LL, 6LL);

    } else if (strcmp(argv[1], "test3") == 0) {
        func = load_function(handle, "test3");

        if (!func) {
            close_object(handle);
            return 1;
        }

        call(func,
             "%i128 %i128 %i128",
             (__int128_t)1, (__int128_t)2, (__int128_t)3);

    } else if (strcmp(argv[1], "test4") == 0) {
        func = load_function(handle, "test4");

        if (!func) {
            close_object(handle);
            return 1;
        }

        call(func,
             "%i32 %i32 %i32 %i32 %i32 %i32 %i32 %i32 %i32 %i32",
             1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

    } else if (strcmp(argv[1], "test5") == 0) {
        func = load_function(handle, "test5");

        if (!func) {
            close_object(handle);
            return 1;
        }

        call(func,
             "%i64 %i128 %i128 %i128 %i64",
             1LL, (__int128_t)2, (__int128_t)3, (__int128_t)4, 5LL);

    } else if (strcmp(argv[1], "test6") == 0) {
        func = load_function(handle, "test6");

        if (!func) {
            close_object(handle);
            return 1;
        }

        call(func,
             "%f64 %f64 %f64 %f64 %f64 %f64 %f64 %f64 %f64",
             0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8);

    } else if (strcmp(argv[1], "test7") == 0) {
        func = load_function(handle, "test7");

        if (!func) {
            close_object(handle);
            return 1;
        }

        call(func,
             "%f64 %f64 %f64 %f64 %f64 %f64 %f64 %f64 %f64 %f64 %f64",
             0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0);

    } else if (strcmp(argv[1], "test8") == 0) {
        func = load_function(handle, "test8");

        if (!func) {
            close_object(handle);
            return 1;
        }

        call(func,
             "%f128 %f128 %f128",
             (long double)0.1, (long double)0.2, (long double)0.3);

    } else if (strcmp(argv[1], "test9") == 0) {
        func = load_function(handle, "test9");

        if (!func) {
            close_object(handle);
            return 1;
        }

        call(func,
             "%f64 %i32 %f64 %i32 %f64 %i32 %f64 %i32 %f64 %i32 %f64 %i32 %f64 %i32 %f64 %i32 %f64 %i32 %f64",
             0.1, 1, 0.2, 2, 0.3, 3, 0.4, 4, 0.5, 5, 0.6, 6, 0.7, 7, 0.8, 8, 0.9, 9, 1.0);

    }

    return 0;
}
