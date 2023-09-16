#include "../include/settings.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int status;
    char *details;
    char *format_string;
    void *args;
} ParsedInfo;

typedef struct {
    size_t size;
    size_t length;
    uint8_t *data;
    size_t format_size;
    size_t format_length;
    char *format_string;
} Args;

Settings settings = {
    .program_name = NULL,
    .files = NULL,
};

FileList *init_file_list()
{
    FileList *files = (FileList*)malloc(sizeof(FileList));
    files->size = 16;
    files->length = 0;
    files->files = (char**)malloc(files->size * sizeof(char*));

    return files;
}

void add_file(FileList *files, const char *file)
{
    if (files->length >= files->size) {
        files->size *= 2;
        files->files = (char**)realloc(files->files, files->size * sizeof(char*));
    }
    files->files[files->length++] = strdup(file);
}

void free_file_list(FileList *files)
{
    for (int i = 0; i < files->length; i++)
        free(files->files[i]);
    free(files->files);
    free(files);
}

FunctionList *init_function_list()
{
    FunctionList *functions = (FunctionList*)malloc(sizeof(FunctionList));
    functions->size = 16;
    functions->length = 0;
    functions->functions = (Function*)malloc(functions->size * sizeof(Function));

    return functions;
}

static ParsedInfo parser(const char *format_string)
{
    ParsedInfo info;
    info.status = 0;
    info.details = NULL;
    info.format_string = NULL;
    info.args = NULL;

    Args args;
    args.size = 16;
    args.length = 0;
    args.data = (uint8_t*)malloc(args.size * sizeof(uint8_t));
    args.format_size = 16;
    args.format_length = 0;
    args.format_string = (char*)malloc(args.format_size * sizeof(char));

    char *input = strdup(format_string);
    char *token = strtok(input, " ");

#define PARSE(t, f)                                                             \
    if (*err) {                                                                 \
        info.status = 3;                                                        \
        info.details = strdup(data);                                            \
        free(type);                                                             \
        free(data);                                                             \
        free(args.format_string);                                               \
        free(args.data);                                                        \
        return info;                                                            \
    }                                                                           \
    if (args.length + sizeof(t) > args.size) {                                  \
        args.size *= 2;                                                         \
        args.data = (uint8_t*)realloc(args.data, args.size * sizeof(uint8_t));  \
    }                                                                           \
    memcpy(args.data + args.length, &value, sizeof(t));                         \
    args.length += sizeof(t);                                                   \
    char *format = f;                                                           \
    size_t s = strlen(format) + 1;                                              \
    if (args.format_length + s > args.format_size) {                            \
        args.format_size *= 2;                                                  \
        args.format_string =                                                    \
            (char*)realloc(args.format_string, args.format_size * sizeof(char));\
    }                                                                           \
    memcpy(args.format_string + args.format_length, f, s);                      \
    args.format_length += s - 1;\

    while (token != NULL) {
        char *type = malloc(strlen(token) + 1);
        char *data = malloc(strlen(token) + 1);

        if (sscanf(token, "(%[^)])%s", type, data) == 2) {
            if (strcmp(type, "int8") == 0 || strcmp(type, "uint8") == 0) {
                char *err = NULL;
                int8_t value = (int8_t)strtoll(data, &err, 0);
                PARSE(int8_t, "%i8");
            } else if (strcmp(type, "int16") == 0 || strcmp(type, "uint16") == 0) {
                char *err = NULL;
                int16_t value = (int16_t)strtoll(data, &err, 0);
                PARSE(int16_t, "%i16");
            } else if (strcmp(type, "int32") == 0 || strcmp(type, "uint32") == 0) {
                char *err = NULL;
                int32_t value = (int32_t)strtoll(data, &err, 0);
                PARSE(int32_t, "%i32");
            } else if (strcmp(type, "int64") == 0 || strcmp(type, "uint64") == 0) {
                char *err = NULL;
                int64_t value = (int64_t)strtoll(data, &err, 0);
                PARSE(int64_t, "%i64");
                } else if (strcmp(type, "int128") == 0 || strcmp(type, "uint128") == 0) {
                char *err = NULL;
                __int128_t value = (__int128_t)strtoll(data, &err, 0);
                PARSE(__int128_t, "%i128");
            } else if (strcmp(type, "float32") == 0) {
                char *err = NULL;
                float value = (float)strtof(data, &err);
                PARSE(float, "%f32");
            } else if (strcmp(type, "float64") == 0) {
                char *err = NULL;
                double value = (double)strtod(data, &err);
                PARSE(double, "%f64");
            } else if (strcmp(type, "float128") == 0) {
                char *err = NULL;
                long double value = (long double)strtold(data, &err);
                PARSE(long double, "%f128");
            } else {
                info.status = 2;
                info.details = strdup(type);
                free(type);
                free(data);
                free(args.format_string);
                free(args.data);
                return info;
            }
        } else {
            free(args.format_string);
            free(args.data);
            info.status = 1;
            info.details = strdup(token);
            return info;
        }

        free(type);
        free(data);

        token = strtok(NULL, " ");
    }

    free(input);

    info.format_string = args.format_string;
    info.args = args.data;

    return info;
}

AddInfo add_function(FunctionList *function_list, size_t file_id, const char *name, const char *format_string)
{
    AddInfo info;
    info.status = 0;
    info.details = 0;

    ParsedInfo parsed = parser(format_string);
    if (parsed.status) {
        info.status = parsed.status;
        info.details = parsed.details;
        return info;
    }

    if (function_list->length >= function_list->size) {
        function_list->size *= 2;
        function_list->functions =
            (Function*)realloc(function_list->functions, function_list->size * sizeof(Function));
    }
    Function *function = &function_list->functions[function_list->length++];
    function->name = strdup(name);
    function->file_id = file_id;

    function->format_string = parsed.format_string;
    function->args = parsed.args;

    return info;
}

void free_function_list(FunctionList *functions)
{
    for (int i = 0; i < functions->length; i++) {
        if (functions->functions[i].name)
            free(functions->functions[i].name);
        if (functions->functions[i].format_string)
            free(functions->functions[i].format_string);
        if (functions->functions[i].args)
            free(functions->functions[i].args);
    }
    free(functions->functions);
    free(functions);
}
