#include "../include/loader.h"

void *load_object(const char *path)
{
    return dlopen(path, RTLD_LAZY);
}

void *load_function(void *handle, const char *name)
{   
    return dlsym(handle, name);
}

void close_object(void *handle)
{
    dlclose(handle);
}
