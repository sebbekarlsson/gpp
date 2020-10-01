#include "include/escape.h"
#include <string.h>
#include <stdlib.h>

static char* mkstr(const char* src)
{
  char* newstr = (char*) calloc(strlen(src) + 1, sizeof(char));
  strcpy(newstr, src);

  return newstr;
}

char* escape_html(char* src)
{
  char* result = (char*) calloc(1, sizeof(char));
  result[0] = '\0';

  for (int i = 0; i < (int) strlen(src); i++)
  {
    char c = src[i];
    const char* placeholder = 0;
    char* newstr = 0;

    switch (c)
    {
      case '&': placeholder = "&amp;"; break;
      case '\"': placeholder = "&quot;"; break;
      case '\'': placeholder = "&apos;"; break;
      case '<': placeholder = "&lt;"; break;
      case '>': placeholder = "&gt;"; break;
      default:  placeholder = 0; break;
    }

    if (placeholder)
    {
      newstr = mkstr(placeholder);
      result = (char*) realloc(result, (strlen(result) + strlen(newstr) + 1) * sizeof(char));
      strcat(result, newstr);
    }
    else
    {
      result = (char*) realloc(result, (strlen(result) + 2) * sizeof(char));
      strcat(result, (char[]){c, '\0'});
    }
  }

  return result;
}
