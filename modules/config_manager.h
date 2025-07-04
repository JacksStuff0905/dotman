#ifndef CONFIG_MANAGER_H_
#define CONFIG_MANAGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <string.h>
#include <stdbool.h>

#include <pwd.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


#ifdef _WIN32
  #define PATH_SEP '\\'
#else
  #define PATH_SEP '/'
#endif

#define DEFAULT_CONFIG "root=/\nhome=$HOME/\n.=$HOME/"

char* read_config(const char* directory);

char* join_path(const char* dir, const char* filename);


char* parse_alias(const char* alias);
char* ltrim(char* s);
void chomp(char* s);


char* parse_env_vars(const char* input);

char* read_env_from_user_proc(const char* varname, uid_t target_uid);

#endif // CONFIG_MANAGER_H_
