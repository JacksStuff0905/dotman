#include "config_manager.h"

char* current_config;

char* read_config(const char* directory) {
  FILE *fp;

  const char* filename = ".dotman";

  char* path = join_path(directory, filename);

  fp = fopen(path, "r"); free(path);

  char* buffer;

  if (fp) {
    if (fseek(fp, 0, SEEK_END) != 0) {
      perror("fseek");
      fclose(fp);
      return NULL;
    }

    long length = ftell(fp);
    if (length < 0) {
      perror("ftell");
      fclose(fp);
      return NULL;
    }

    rewind(fp);

    buffer = malloc(length + 1); // +1 for null terminator
    if (!buffer) {
      perror("malloc");
      fclose(fp);
      return NULL;
    }

    fread(buffer, 1, length, fp);
    buffer[length] = '\0';

    fclose(fp);

  } else {
    printf("Dotman file(.dotman) doesn't exist, using default config\n");
    buffer = DEFAULT_CONFIG;
  }

  current_config = buffer;
  return buffer;
}


char* join_path(const char* dir, const char* filename) {
  size_t len_dir = strlen(dir);
  size_t len_file = strlen(filename);

  int needs_sep = (len_dir > 0 && dir[len_dir - 1] != PATH_SEP);


  // +1 for separator if needed, +1 for null terminator
  char* result = malloc(len_dir + len_file + (needs_sep ? 2 : 1));
  if (!result) {
    perror("malloc");
    return NULL;
  }

  strcpy(result, dir);
  if (needs_sep) {
    size_t pos = strlen(result);
    result[pos] = PATH_SEP;
    result[pos + 1] = '\0';
  }
  strcat(result, filename);
  
  return result;
}

char* ltrim(char* s) {
  while (isspace((unsigned char)*s)) s++;
  return s;
}

void chomp(char* s) {
  size_t len = strlen(s);
  while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
    s[--len] = '\0';
  }
}

char* parse_alias(const char* alias) {
  // Copy string
  char* copy = malloc(strlen(current_config) + 1);
  if (!copy) {
    perror("malloc");
    return NULL;
  }
  strcpy(copy, current_config);

  char* line = copy;

  char* default_alias;

  while (line && *line) {
    char *next = strchr(line, '\n');
    if (next) {
      *next = '\0';
      next++;
    }

    chomp(line);
    char* trimmed = ltrim(line);

    if (*trimmed != '\0' && *trimmed != '#') {
      char* equal = strchr(trimmed, '=');
      if (equal) {
        *equal = '\0';
        char* key = ltrim(trimmed);
        char* value = ltrim(equal + 1);
        if (strcmp(key, alias) == 0) {
          // Alias match
          char* new_value = malloc(strlen(value) + 1);
          strcpy(new_value, value);
          free(copy);
          
          return parse_env_vars(new_value);
        } else if (strcmp(key, ".") == 0) {
          // Default alias
          default_alias = parse_env_vars(value);
        }
      }
    }

    line = next;
  }

  // Falling back to the default alias
  //free(copy);
  char* new_alias = malloc(strlen(alias) + 1);
  strcpy(new_alias, alias);
  free(copy);
  return join_path(default_alias, new_alias);
}



char* parse_env_vars(const char* input) {
  char* copy = malloc(strlen(input) + 1);
  char* result = malloc(0);
  char* tmp_env = malloc(0);
  strcpy(copy, input);

  bool is_processing_env = false;
  int count = 0;
  while (*copy != '\0') {
    if (!is_processing_env && tmp_env != NULL && strlen(tmp_env) > 0) {
      char* env = getenv(tmp_env);


      if (!result)
        result = malloc(strlen(env) + 2);
      else
        result = realloc(result, strlen(result) + strlen(env) + 2);
      strcat(result, env);
      free(tmp_env);
    }

    if (*copy == '$') {
      is_processing_env = true;
    } else if (is_processing_env) {
      if (*copy == PATH_SEP) {
        is_processing_env = false;

        if (!result) 
          result = (char*)malloc(2);
        else
          result = (char*)realloc(result, strlen(result) + 2);
        result[strlen(result)] = *copy;
        result[strlen(result) + 1] = '\0';
      } else {
        if (!tmp_env)
          tmp_env = (char*)malloc(2);
        else
          tmp_env = (char*)realloc(tmp_env, strlen(tmp_env) + 2);
        tmp_env[strlen(tmp_env)] = *copy;
        tmp_env[strlen(tmp_env) + 1] = '\0';
      }
    } else {

      if (!result)
        result = (char*)malloc(2);
      else
        result = (char*)realloc(result, strlen(result) + 2);
      result[strlen(result)] = *copy;
      result[strlen(result) + 1] = '\0';
    }
    copy++;
    count++;
  }

  if (tmp_env && strlen(tmp_env) > 0)
    strcat(result, getenv(tmp_env));

  copy -= count;
  free(copy);
  
  if (tmp_env)
    free(tmp_env);

  return result;
}
