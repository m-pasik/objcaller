#include "../include/settings.h"

Settings settings = {
    .program_name = NULL,
    .files = NULL
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
