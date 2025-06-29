#include "config_manager.h"

char* current_config;

char* read_config(const char* directory) {
  FILE *fp;

  const char* filename = ".dotman";

  char* path = join_path(directory, filename);

  fp = fopen(path, "r");

  free(path);

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
          free(copy);
          return value;
        } else if (strcmp(key, ".") == 0) {
          // Default alias
          default_alias = value;
        }
      }
    }

    line = next;
  }

  // Falling back to the default alias
  free(copy);
  return join_path(default_alias, alias);
}
