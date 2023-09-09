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
    printf("Usage: %s [options]\n"
           "\n"
           "Options:\n"
           "  --help                            -h  -- Displays help.\n"
           "  --file <file>                     -f  -- Open object file\n"
           "  --call <index> <function> <args>  -c  -- Call a function in a file\n"
           "\n"
           "Calling functions:\n"
           "  First argument to --call function is an index of the object.\n"
           "  file containing a function you want to call.\n"
           "  Second argument is the name of the function.\n"
           "  Third is a string containing the list of arguments.\n"
           "\n"
           "  String format:\n"
           "    \"(type)arg1 (type)arg2 ...\"\n"
           "\n"
           "  Supported types:\n"
           "    int8     - 8 bit integer\n"
           "    int16    - 16 bit integer\n"
           "    int32    - 32 bit integer\n"
           "    int64    - 64 bit integer\n"
           "    int128   - 128 bit integer\n"
           "    uint8    - 8 bit unsigned integer\n"
           "    uint16   - 16 bit unsigned integer\n"
           "    uint32   - 32 bit unsigned integer\n"
           "    uint64   - 64 bit unsigned integer\n"
           "    uint128  - 128 bit unsigned integer\n"
           "    float32  - 32 bit float\n"
           "    float64  - 64 bit float\n"
           "    float128 - 128 bit float\n",
           settings.program_name);
    exit(0);
    return 0;
}

/*
 * Adds file to the list.
 */
int file(size_t argc, char **argv)
{
    if (!settings.files)
        settings.files = init_file_list();

    add_file(settings.files, argv[0]);

    return 0;
}

/*
 * Add function to the list.
 */
int call_func(size_t argc, char **argv)
{
    /*
     * To be implemented.
     */

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

    add_option(options, "call", 'c', 3, 3, call_func);

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

