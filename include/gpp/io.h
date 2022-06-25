#ifndef GPP_IO_H
#define GPP_IO_H
#include <stdio.h>
#include <stdlib.h>

char *gpp_read_file(const char *filename);
unsigned int gpp_file_exists(const char *path);
#endif
