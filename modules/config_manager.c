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
  
  printf("%s\n", parse_env_vars(buffer));
  buffer = parse_env_vars(buffer);

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

        printf("key: %s\n", key);
        printf("value: %s\n", value);
        if (strcmp(key, alias) == 0) {
          // Alias match
          char* new_value = malloc(strlen(value) + 1);
          strcpy(new_value, value);
          free(copy);
          
          return (new_value);
        } else if (strcmp(key, ".") == 0) {
          // Default alias
          char* new_value = malloc(strlen(value) + 1);
          strcpy(new_value, value);
          free(copy);

          default_alias = new_value;
          printf("Setting default %s\n", default_alias);
        }
      }
    }

    line = next;
  }
  printf("test1\n");

  // Falling back to the default alias
  //free(copy);
  char* new_alias = malloc(strlen(alias) + 1);
  strcpy(new_alias, alias);
  free(copy);
  printf("Using default %s %s\n", default_alias, join_path(default_alias, new_alias));
  return join_path(default_alias, new_alias);
}



char* parse_env_vars(const char* input) {
  char* copy = malloc(strlen(input) + 1);
  char* result = malloc(0);
  char* tmp_env = malloc(0);
  strcpy(copy, input);


  /*
  const char *sudo_user = getenv("SUDO_USER");
  if (!sudo_user) {
      fprintf(stderr, "Run with sudo.\n");
      return NULL;
  }

  struct passwd *pw = getpwnam(sudo_user);
  if (!pw) {
      perror("getpwnam");
      return NULL;
  }
  */


  bool is_processing_env = false;
  int count = 0;
  while (*copy != '\0') {
    
    printf(" %c\n", *copy);
    if (*copy == '$') {
      is_processing_env = true;
    } else if (is_processing_env) {
      if (*copy == PATH_SEP || *copy == '\n') {
        is_processing_env = false;

        printf("1111\n");
        char* env = getenv(tmp_env);//, pw->pw_uid);
        printf("2222\n");

        if (!result)
          result = malloc(strlen(env) + 2);
        else
          result = realloc(result, strlen(result) + strlen(env) + 2);
        strcat(result, env);
        free(tmp_env);
        tmp_env= malloc(0);

        printf("5555\n");

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
    printf("3333\n");

    copy++;
    count++;
  }
  printf("test5\n");

  if (tmp_env && strlen(tmp_env) > 0)
    strcat(result, getenv(tmp_env));//, pw->pw_uid));

  copy -= count;
  free(copy);
  
  if (tmp_env)
    free(tmp_env);

  return result;
}



char *read_env_from_user_proc(const char *varname, uid_t target_uid) {
    DIR *proc = opendir("/proc");
    if (!proc) return NULL;

    struct dirent *entry;
    while ((entry = readdir(proc)) != NULL) {
        if (!isdigit(entry->d_name[0])) continue;

        char path[256];
        snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);
        FILE *status = fopen(path, "r");
        if (!status) continue;

        uid_t uid = -1;
        char line[256];
        while (fgets(line, sizeof(line), status)) {
            if (sscanf(line, "Uid:\t%d", &uid) == 1) break;
        }
        fclose(status);

        if (uid != target_uid) continue;

        // Found a process from the target user
        snprintf(path, sizeof(path), "/proc/%s/environ", entry->d_name);
        int fd = open(path, O_RDONLY);
        if (fd == -1) continue;

        char buffer[8192];
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
        close(fd);
        if (n <= 0) continue;

        buffer[n] = '\0';

        // Look for VAR=...
        char *ptr = buffer;
        while (ptr < buffer + n) {
            size_t len = strlen(ptr);
            if (strncmp(ptr, varname, strlen(varname)) == 0 && ptr[strlen(varname)] == '=') {
                closedir(proc);
                return strdup(ptr + strlen(varname) + 1);
            }
            ptr += len + 1;
        }
    }

    closedir(proc);
    return NULL;
}
