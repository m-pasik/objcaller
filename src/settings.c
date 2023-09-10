#include "../include/settings.h"

typedef struct {
    int status;
    char *details;
    char *format_string;
    void *args;
} ParsedInfo;

typedef struct {
    size_t size;
    char *data;
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

ParsedInfo parser(const char *format_string)
{
    ParsedInfo info;
    info.status = 0;
    info.details = NULL;
    info.format_string = NULL;
    info.args = NULL;

    Args args;
    args.size = 0;
    args.data = NULL;

    for (const char *p = format_string, *c = strchr(format_string, '(');
            c != NULL;
            p = c, c = strchr(++c, '(')) {
        for ( ; p < c; p++) {
            if (*p != ' ' && *p != '\t') {
                info.status = 1;
                info.details = malloc(sizeof(char) * 2);
                info.details[0] = *p;
                info.details[1] = '\0';
                return info;
            }
        }

        p = ++c;
        const char *p2 = p;
        if ((c = strchr(p, ')')) == NULL) {
            info.status = 2;
            info.details = NULL;
            return info;
        }

        char type;
        if (strncmp(p, "int", 3) == 0) {
            type = 'i';
            p2 += 3; 
        } else if (strncmp(p, "uint", 4) == 0) {
            type = 'u';
            p2 += 4;
        } else if (strncmp(p, "float", 5) == 0) {
            type = 'f';
            p2 += 5;
        } else {
            info.status = 3;
            info.details = malloc(sizeof(char) * (c - p));
            memcpy(info.details, p, c - p);
            return info;
        }

        //for ( ; p2 < c; p2++) {
        //    if (*p2 != ' ' && *p2 != '\t') {
        //        info.status = 3;
        //        info.details = malloc(sizeof(char) * (c - p));
        //        memcpy(info.details, p, c - p);
        //        return info;
        //    }
        //}

        switch (type) {
            case 'i':
                break;
            case 'u':
                break;
            case 'f':
                break;
        }

    }

    return info;
}

AddInfo add_function(FunctionList *functions, size_t file_id, const char *name, const char *format_string)
{
    AddInfo info;
    info.status = 0;
    info.details = 0;

    if (functions->length >= functions->size) {
        functions->size *= 2;
        functions->functions = (Function*)realloc(functions->functions, functions->size * sizeof(Function));
    }
    Function *function = &functions->functions[functions->length++];
    function->name = strdup(name);
    function->file_id = file_id;

    ParsedInfo parsed = parser(format_string);
    if (parsed.status) {
        info.status = parsed.status;
        info.details = parsed.details;
        return info;
    }

    function->format_string = parsed.format_string;
    function->args = parsed.args;

    return info;
}

void free_function_list(FunctionList *functions)
{
    for (int i = 0; i < functions->length; i++) {
        free(functions->functions[i].name);
        free(functions->functions[i].format_string);
        free(functions->functions[i].args);
    }
    free(functions->functions);
    free(functions);
}
