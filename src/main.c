#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>

#include "../include/call.h"
#include "../include/loader.h"
#include "../include/options.h"
#include "../include/settings.h"

/*
 * Displays help
 */
int help(size_t argc, char **argv)
{
    printf("Usage: %s [options]\n\n", settings.program_name);
    printf("Options:\n"
           "  --help        -h  -- Displays help.\n");
    exit(0);
    return 0;
}

int file(size_t argc, char **argv)
{
    if (!settings.files)
        settings.files = init_file_list();

    add_file(settings.files, argv[0]);

    return 0;
}

int main(int argc, char **argv)
{
    settings.program_name = argv[0];

    if (argc < 2)
        help(0, NULL);

    /*
     * Initialize `Options` struct
     * and declare command line arguments.
     */

    Options *options = init_options();

    add_option(options, "help", 'h', 0, 0, help);

    add_option(options, "file", 'f', 1, 1, file);


    char **args = argv + 1;
    size_t arg_count = argc - 1;

    /*
     * Parse command line arguments
     */
    for (ArgInfo *arg = parse_argument(options, &arg_count, &args);
         arg;
         arg = parse_argument(options, &arg_count, &args)) {
        
        /* Exif if invalid argument was provided. */
        if (!arg->option) {
            fprintf(stderr, "ERROR: Invalid argument %s\n", arg->opt_argv[0]);
            exit(1);
        }

        /* Call function associated with the provided argument. */
        arg->option->call(arg->opt_argc, arg->opt_argv);

        free(arg);
    }

    free_options(options);

    if (settings.files)
        free_file_list(settings.files);

    return 0;
}

