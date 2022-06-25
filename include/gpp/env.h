#ifndef GPP_ENV_H
#define GPP_ENV_H
#include <hashmap/map.h>
typedef struct {
  const char* base_path;
  char* base_dir;
  map_T* includes;
} GPPEnv;
#endif
