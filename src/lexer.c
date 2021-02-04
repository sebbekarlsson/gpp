#include "include/lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *charstr(char c) {
  char *new_str = (char *)calloc(2, sizeof(char));
  new_str[0] = c;
  new_str[1] = '\0';

  return new_str;
}

lexer_T *init_lexer(char *src) {
  lexer_T *lexer = (lexer_T *)calloc(1, sizeof(struct LEXER_STRUCT));

  if (src) {
    lexer->src = (char *)calloc(strlen(src) + 1, sizeof(char));
    strcpy(lexer->src, src);
  } else {
    lexer->src = (char *)calloc(2, sizeof(char));
    lexer->src[0] = '\0';
    lexer->src[1] = '\0';
  }

  lexer->i = 0;
  lexer->c = lexer->src[lexer->i];
  lexer->len = strlen(lexer->src);
  lexer->raw = 1;
  lexer->comp = 0;
  lexer->raws = 0;
  lexer->x = 0;
  lexer->y = 0;

  return lexer;
}

void lexer_advance(lexer_T *lexer) {
  if (lexer->c != '\0' && lexer->i < lexer->len) {
    if (lexer->c == '\n') {
      lexer->y += 1;
      lexer->x = 0;
    } else {
      lexer->x += 1;
    }
    lexer->i = lexer->i + 1;
    lexer->c = lexer->src[lexer->i];
  } else {
    printf("[Lexer]: Cannot advance anymore.\n");
  }
}

token_T *lexer_advance_token(lexer_T *lexer, token_T *token) {
  lexer_advance(lexer);
  return token;
}

void lexer_skip_whitespace(lexer_T *lexer) {
  while ((lexer->c == ' ' || lexer->c == 10 || lexer->c == '\n') &&
         (lexer->i < lexer->len))
    lexer_advance(lexer);
}

token_T *lexer_parse_left_paren(lexer_T *lexer) {
  lexer_advance(lexer);

  return init_token(charstr(lexer->c), TOKEN_LPAREN);
}

token_T *lexer_parse_right_paren(lexer_T *lexer) {
  char *value = charstr(lexer->c);
  return init_token(value, TOKEN_RPAREN);
}

token_T *lexer_parse_id(lexer_T *lexer) {
  char *value = (char *)calloc(1, sizeof(char));
  value[0] = '\0';

  do {
    value = (char *)realloc(value, (strlen(value) + 2) * sizeof(char));
    strcat(value, charstr(lexer->c));
    lexer_advance(lexer);
  } while (lexer->c != '\0' && (isalpha(lexer->c) || isdigit(lexer->c)));

  return init_token(value, TOKEN_ID);
}

token_T *lexer_parse_string(lexer_T *lexer) {
  lexer_advance(lexer);

  char *value = (char *)calloc(1, sizeof(char));
  value[0] = '\0';

  do {
    value = (char *)realloc(value, (strlen(value) + 2) * sizeof(char));
    strcat(value, charstr(lexer->c));
    lexer_advance(lexer);
  } while (lexer->c != '\0' && lexer->c != '"');

  return init_token(value, TOKEN_STRING);
}

token_T *lexer_parse_number(lexer_T *lexer) {
  char *value = (char *)calloc(1, sizeof(char));
  value[0] = '\0';

  do {
    value = (char *)realloc(value, (strlen(value) + 2) * sizeof(char));
    strcat(value, charstr(lexer->c));
    lexer_advance(lexer);
  } while (lexer->c != '\0' && isdigit(lexer->c));

  if (lexer->c == '.') {
    value = (char *)realloc(value, (strlen(value) + 2) * sizeof(char));
    strcat(value, charstr(lexer->c));
    lexer_advance(lexer);

    do {
      value = (char *)realloc(value, (strlen(value) + 2) * sizeof(char));
      strcat(value, charstr(lexer->c));
      lexer_advance(lexer);
    } while (lexer->c != '\0' && isdigit(lexer->c));
  }

  return init_token(value, TOKEN_NUMBER);
}

token_T *lexer_parse_comment(lexer_T *lexer) {
  lexer_advance(lexer);

  char *value = (char *)calloc(1, sizeof(char));
  value[0] = '\0';

  do {
    value = (char *)realloc(value, (strlen(value) + 2) * sizeof(char));
    strcat(value, charstr(lexer->c));
    lexer_advance(lexer);
  } while (lexer->c != '\0' && lexer->c != '\n' && lexer->c != 10);

  return init_token(value, TOKEN_COMMENT);
}

token_T *lexer_parse_arrow_right(lexer_T *lexer) {
  token_T *token = init_token(charstr(lexer->c), TOKEN_ARROW_RIGHT);
  lexer_advance(lexer); // =
  lexer_advance(lexer); // >

  return token;
}

token_T *lexer_parse_any(lexer_T *lexer) {
  lexer_skip_whitespace(lexer);

  char *value = (char *)calloc(1, sizeof(char));
  value[0] = '\0';

  while (lexer->c != '\0' &&
         (lexer->y <= 1 ? lexer->c != '#' : lexer->c != '\0') &&
         !(lexer->c == '{' && lexer_peek(lexer, 1) == '{') &&
         !(lexer->c == VFB0 && lexer_peek(lexer, 1) == VFB1)) {
    if ((lexer->c == VFE0 && lexer_peek(lexer, 1) == VFE1)) {
      lexer_advance(lexer);
      lexer_advance(lexer);
    } else {
      value = (char *)realloc(value, (strlen(value) + 2) * sizeof(char));
      strcat(value, charstr(lexer->c));
      lexer_advance(lexer);
    }
  }

  return init_token(value, TOKEN_STRING);
}

token_T *lexer_parse_raw_begin(lexer_T *lexer) {
  lexer_advance(lexer);
  lexer_skip_whitespace(lexer);
  lexer_advance(lexer);

  return init_token(charstr(lexer->c), TOKEN_RAW_BEGIN);
}

token_T *lexer_parse_raw_end(lexer_T *lexer) {
  lexer_advance(lexer);
  lexer_skip_whitespace(lexer);
  lexer_advance(lexer);

  return init_token(charstr(lexer->c), TOKEN_RAW_END);
}

token_T *lexer_parse_comp_begin(lexer_T *lexer) {
  char *value = charstr(lexer->c);

  lexer_advance(lexer);
  lexer_skip_whitespace(lexer);
  lexer_advance(lexer);

  lexer->comp = 1;

  return init_token(value, TOKEN_COMP_BEGIN);
}

token_T *lexer_parse_comp_end(lexer_T *lexer) {
  char *value = charstr(lexer->c);

  lexer_advance(lexer);
  lexer_skip_whitespace(lexer);
  lexer_advance(lexer);

  lexer->comp = 0;

  return init_token(value, TOKEN_COMP_END);
}

token_T *lexer_parse_raw(lexer_T *lexer, unsigned int all) {
  int open_count = 0;
  int lcount = 0;
  int close_count = 0;
  unsigned int shift = 0;

  unsigned int x = lexer->x;

  char *value = (char *)calloc(1, sizeof(char));
  value[0] = '\0';
  open_count += (lexer->c == VFB0 && lexer_peek(lexer, 1) == VFB1);
  close_count += (lexer->c == VFE0 && lexer_peek(lexer, 1) == VFE1);
  lcount += (lexer->c == '(');

  while (lexer->c != '\0') {
    if (lexer->y <= 1 && lexer->c == '#')
      break;

    if (!all) {
      open_count += (lexer->c == VFB0 && lexer_peek(lexer, 1) == VFB1);
      close_count += (lexer->c == VFE0 && lexer_peek(lexer, 1) == VFE1);

      if ((close_count >= open_count) && open_count > 0)
        break;

      if (open_count >= 1) {
        if (!shift) {
          lexer_advance(lexer);
          lexer_advance(lexer);
          lexer->raw = 1;
          shift = 1;
        }

        if ((close_count + (lexer->c == VFE0 && lexer_peek(lexer, 1) == VFE1) >=
             open_count)) {
          break;
        }

        value = (char *)realloc(value, (strlen(value) + 2) * sizeof(char));
        strcat(value, charstr(lexer->c));
      }

      lcount += (lexer->c == VFB0);

      lexer_advance(lexer);
    } else {
      value = (char *)realloc(value, (strlen(value) + 2) * sizeof(char));
      strcat(value, charstr(lexer->c));
      lexer_advance(lexer);
    }
  }

  lexer_skip_whitespace(lexer);

  if (lexer->c == VFE0 || lexer->c == VFE1) {
    lexer_advance(lexer);
    lexer_skip_whitespace(lexer);
  }
  if (lexer->c == VFE0 || lexer->c == VFE1) {
    lexer_advance(lexer);
    lexer_skip_whitespace(lexer);
  }

  lexer->raw = 0;
  lexer->raws += 1;

  token_T *token = init_token(value, TOKEN_RAW);
  token->x = x;
  return token;
}

static token_T *lexer_char_to_token(lexer_T *lexer) {
  if (lexer->c == '=' && lexer_peek(lexer, 1) == '>')
    return lexer_parse_arrow_right(lexer);

  switch (lexer->c) {
  case '(':
    return lexer_parse_left_paren(lexer);
    break;
  case ')':
    return lexer_advance_token(lexer, lexer_parse_right_paren(lexer));
    break;
  case '=':
    return lexer_advance_token(lexer,
                               init_token(charstr(lexer->c), TOKEN_EQUALS));
    break;
  case '"':
    return lexer_advance_token(lexer, lexer_parse_string(lexer));
    break;
  case '[':
    return lexer_advance_token(lexer,
                               init_token(charstr(lexer->c), TOKEN_LBRACKET));
    break;
  case ']':
    return lexer_advance_token(lexer,
                               init_token(charstr(lexer->c), TOKEN_RBRACKET));
    break;
  case '#':
    return lexer_parse_comment(lexer);
    break;
  case '.':
    return lexer_advance_token(lexer, init_token(charstr(lexer->c), TOKEN_DOT));
    break;
  case ',':
    return lexer_advance_token(lexer,
                               init_token(charstr(lexer->c), TOKEN_COMMA));
    break;
  default:
    return lexer_parse_raw(lexer, 1);
    break;
  }
}

token_T *lexer_next_token_simple(lexer_T *lexer) {
  while (lexer->c != '\0') {
    lexer_skip_whitespace(lexer);

    while (lexer->c == '%')
      lexer_advance(lexer);

    if (isalpha(lexer->c) || lexer->c == '$')
      return lexer_parse_id(lexer);

    return lexer_char_to_token(lexer);
  }

  return init_token(charstr(lexer->c), TOKEN_EOF);
}

token_T *lexer_next_token(lexer_T *lexer) {
  while (lexer->c != '\0') {
    while (lexer->c == '{' && lexer_peek(lexer, 1) == '{')
      return lexer_parse_comp_begin(lexer);

    if (!lexer->comp && lexer->c != '#' &&
        !(lexer->c == '{' && lexer_peek(lexer, 1) == '{') &&
        !(lexer->c == VFB0 && lexer_peek(lexer, 1) == VFB1))
      return lexer_parse_any(lexer);

    if (lexer->comp)
      lexer_skip_whitespace(lexer);

    while (lexer->c == '}' && lexer_peek(lexer, 1) == '}' && lexer->c != '.')
      return lexer_parse_comp_end(lexer);

    if (lexer->c == VFB0 && lexer_peek(lexer, 1) == VFB1)
      return lexer_parse_raw(lexer, 0);

    if (lexer->c == '\0')
      break;

    if (lexer->c == '$')
      return lexer_parse_id(lexer);

    if (lexer->c == '\0')
      break;

    if (isdigit(lexer->c))
      return lexer_parse_number(lexer);
    if (isalpha(lexer->c) || lexer->c == '$')
      return lexer_parse_id(lexer);

    return lexer_char_to_token(lexer);
  }

  return init_token(charstr(lexer->c), TOKEN_EOF);
}

char lexer_peek(lexer_T *lexer, unsigned int index) {
  return lexer->i < lexer->len ? lexer->src[lexer->i + index] : '\0';
}
