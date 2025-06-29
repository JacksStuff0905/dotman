#include "config_manager.h"

char* read_config(const char* directory) {
  FILE *fp;

  const char* filename = ".dotman";

  char* path = join_path(directory, filename);

  fp = fopen(path, "r");
  if (!fp) {
    perror("fopen");
    return NULL;
  }


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

  char* buffer = malloc(length + 1); // +1 for null terminator
  if (!buffer) {
    perror("malloc");
    fclose(fp);
    return NULL;
  }

  fread(buffer, 1, length, fp);
  buffer[length] = '\0';

  fclose(fp);

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
