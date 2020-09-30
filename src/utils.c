#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void assert_not_nil(void* x, const char* msg)
{
    if (!!x) return;
    printf("ASSERT FAILED. ITEM IS NIL (%p) - %s\n", x, msg);
    exit(1);
}

void assert_not_reached()
{
    printf("ASSERT FAILED. SHOULD NOT HAVE REACHED HERE.\n");
    exit(1);
}

char* remove_indent(char* instr, unsigned int x)
{
  char* newstr = (char*)calloc(1, sizeof(char));
  newstr[0] = '\0';

  // count number of lines in string
  unsigned int n = 0;
  for (unsigned int i = 0; i < strlen(instr); i++)
    if (instr[i] == 10)
      n++;

  char* tok = strtok(instr, "\n"); 

  unsigned int tokc = 0;
  while (tok != 0)
  {

    // count how much we can remove
    unsigned int i = 0;
    for (; i < x; i++)
      if (tok[i] != ' ')
        break;

    char* right = tok+i;
    newstr = (char*) realloc(newstr, (strlen(newstr) + strlen(right) + 4) * sizeof(char));

    if (n > 1)
      strcat(newstr, (char[2]){'\n', 0});

    strcat(newstr, right);

    if (tokc < n-1)
      strcat(newstr, (char[2]){'\n', 0});

    tok = strtok(0, "\n");
    tokc++;
  }

  return newstr;
}
