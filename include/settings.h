#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t size;
    size_t length;
    char **files;
} FileList;

typedef struct {
    char *program_name;
    FileList *files;
} Settings;

extern Settings settings;

FileList *init_file_list();
void add_file(FileList *files, const char *file);
void free_file_list(FileList *files);

#endif
