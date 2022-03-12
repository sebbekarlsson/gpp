#ifndef GPP_UTILS_H
#define GPP_UTILS_H
#include "AST.h"
#include <stdint.h>
void assert_not_nil(void *x, const char *msg);
void assert_not_reached();

char *remove_indent(char *instr, unsigned int x);

char* gpp_path_join(const char* a, const char* b);
unsigned int gpp_path_is_dir(const char* path);

unsigned int gpp_string_starts_with_char(const char* str, char c);
unsigned int gpp_string_ends_with_char(const char* str, char c);
char *gpp_string_append(char **inputstr, const char *other);
char *gpp_dirname(const char *path);


#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif

#ifndef MIN
#define MIN(a, b) (a < b ? a : b)
#endif
#endif
