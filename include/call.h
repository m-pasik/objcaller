#ifndef CALL_H
#define CALL_H

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>

void call(void* fptr, char *fstring, ...);

#endif
