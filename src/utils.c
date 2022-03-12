#include <gpp/utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void assert_not_nil(void *x, const char *msg) {
  if (!!x)
    return;
  printf("ASSERT FAILED. ITEM IS NIL (%p) - %s\n", x, msg);
  exit(1);
}

void assert_not_reached() {
  printf("ASSERT FAILED. SHOULD NOT HAVE REACHED HERE.\n");
  exit(1);
}

char *remove_indent(char *instr, unsigned int x) {
  char *newstr = (char *)calloc(1, sizeof(char));
  newstr[0] = '\0';

  // count number of lines in string
  unsigned int n = 0;
  for (unsigned int i = 0; i < strlen(instr); i++)
    if (instr[i] == 10)
      n++;

  char *tok = strtok(instr, "\n");

  unsigned int tokc = 0;
  while (tok != 0) {

    // count how much we can remove
    unsigned int i = 0;
    for (; i < x; i++)
      if (tok[i] != ' ')
        break;

    char *right = tok + i;
    newstr = (char *)realloc(newstr, (strlen(newstr) + strlen(right) + 4) *
                                         sizeof(char));

    if (n > 1)
      strcat(newstr, (char[2]){'\n', 0});

    strcat(newstr, right);

    if (tokc < n - 1)
      strcat(newstr, (char[2]){'\n', 0});

    tok = strtok(0, "\n");
    tokc++;
  }

  return newstr;
}


unsigned int gpp_string_starts_with_char(const char *str, char c) {
  if (!str) return 0;
  return str[0] == c;
}
unsigned int gpp_string_ends_with_char(const char *str, char c) {
  if (!str) return 0;
  uint32_t len = strlen(str);
  return str[MAX(0, len - 1)] == c;
}

char *gpp_string_append(char **inputstr, const char *other) {
  if (!inputstr || !*inputstr) {
    *inputstr = (char *)calloc(2, sizeof(char));
  }
  char *str = *inputstr;

  uint32_t old_len = strlen(str);
  uint32_t other_len = strlen(other);
  uint32_t new_len = old_len + other_len;

  str = (char *)realloc(str, (new_len + 1) * sizeof(char));
  strcat(str, other);
  *inputstr = str;

  return str;
}


char* gpp_path_join(const char* a, const char* b) {
  uint32_t len_a = strlen(a);
  uint32_t len_b = strlen(b);
  char buff_a[len_a + 1];
  char buff_b[len_b + 1];
  memcpy(&buff_a[0], a, (len_a + 1) * sizeof(char));
  memcpy(&buff_b[0], b, (len_b + 1) * sizeof(char));

  unsigned int needs_slash = 0;

  if (gpp_string_ends_with_char(a, '/') &&
      gpp_string_starts_with_char(b, '/')) {
    buff_a[len_a - 1] = 0;
  } else if (!gpp_string_ends_with_char(a, '/') &&
             !gpp_string_starts_with_char(b, '/')) {
    needs_slash = 1;
  }

  char* newstr = 0;
  gpp_string_append(&newstr, buff_a);
  if (needs_slash) {
    gpp_string_append(&newstr, "/");
  }
  gpp_string_append(&newstr, buff_b);

  return newstr;
}

unsigned int gpp_path_is_dir(const char* path) {
  return gpp_string_ends_with_char(path, '/');
}

static char *gpp_last_strstr(const char *haystack, const char *needle) {
  if (*needle == '\0') return (char *)haystack;

  char *result = NULL;
  for (;;) {
    char *p = strstr(haystack, needle);
    if (p == NULL) break;
    result = p;
    haystack = p + 1;
  }

  return result;
}

char *gpp_dirname(const char *path) {
  if (!path) goto failure;

  uint32_t len = strlen(path);

  if (!len) goto failure;

  char *last = gpp_last_strstr(path, "/");

  if (!last) goto failure;

  int pos = last - path;

  char *out = (char *)calloc(pos + 2, sizeof(char));

  memcpy(out, &path[0], (pos) * sizeof(char));

  out[pos] = '/';

  return out;

failure : { return strdup(path); }
}
