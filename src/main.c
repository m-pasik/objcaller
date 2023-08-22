#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>

#include "call.h"
#include "loader.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <library_path>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];

    void *handle = load_object(path);
    if (!handle) { 
        printf("Error: %s\n", dlerror());
        return 1;
    }

    void *func = load_function(handle, "test4");
    if (!func) {
        fprintf(stderr, "Error: %s\n", dlerror());
        close_object(handle);
        return 1;
    }

    call(func, 
        "%f64 %f64 %f64 %f64 %f64 %f64 %f64 %f64 %f64"
        "%i32 %i32 %i32 %i32 %i32 %i32 %i32 %i32 %i32",
        0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
        1, 2, 3, 4, 5, 6, 7, 8, 9);

    close_object(handle);

    return 0;
}
