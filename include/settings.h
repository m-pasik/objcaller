#ifndef SETTINGS_H
#define SETTINGS_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t size;
    size_t length;
    char **files;
} FileList;

typedef struct {
    char *name;
    size_t file_id;
    char *format_string;
    void *args;
} Function;

typedef struct {
    size_t size;
    size_t length;
    Function *functions;
} FunctionList;

typedef struct {
    int status;
    char *details;
} AddInfo;

typedef struct {
    char *program_name;
    FileList *files;
    FunctionList *function_list;
} Settings;

extern Settings settings;

FileList *init_file_list();
void add_file(FileList *files, const char *file);
void free_file_list(FileList *files);

FunctionList *init_function_list();
AddInfo add_function(FunctionList *functions, size_t file_id, const char *name, const char *format_string);
void free_function_list(FunctionList *functions);

#endif
