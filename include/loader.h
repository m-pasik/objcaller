#ifndef LOADER_H
#define LOADER_H

void *load_object(const char *path);

void *load_function(void *handle, const char *name);

void close_object(void *handle);

#endif
