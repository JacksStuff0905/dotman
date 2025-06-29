#ifndef CONFIG_MANAGER_H_
#define CONFIG_MANAGER_H_

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#ifdef _WIN32
  #define PATH_SEP '\\'
#else
  #define PATH_SEP '/'
#endif

#define DEFAULT_CONFIG "root=/\nhome=$HOME/\n.=$HOME/"

char* read_config(const char* directory);

char* join_path(const char* dir, const char* filename);

#endif // CONFIG_MANAGER_H_
