#ifndef __DIE_H_X3VetjtyeAY3v2yB
#define __DIE_H_X3VetjtyeAY3v2yB

#include <stdio.h>

#define die(...) do { fprintf(stderr, __VA_ARGS__); exit(1); } while (0);

#endif
