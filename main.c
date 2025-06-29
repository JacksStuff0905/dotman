#include "main.h"

int main(int argc, char* argv[]) {

  int opt;

  char* dir = ".";

  static struct option long_options[] = {
    {"no", no_argument, NULL, 'n'},
    {"dir", required_argument, NULL, 'd'},
    {"target", required_argument, NULL, 't'},
    {"verbose", optional_argument, NULL, 'v'},
    {"stow", no_argument, NULL, 'S'},
    {"delete", no_argument, NULL, 'D'},
    {"restow", no_argument, NULL, 'R'},
    {"version", no_argument, NULL, 'V'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}
  };

  while ((opt = getopt_long(argc, argv, ":nd:t:v::SDRVh", long_options, NULL)) != -1) {
    switch (opt) {
      case 'n':
        printf("Option -n (--no, --simulate) selected\n");
        break;
      case 'd':
        printf("Option -d (--dir) selected with argument %s\n", optarg);
        dir = optarg;
        break;
      case 't':
        printf("Option -t (--target) selected with argument %s\n", optarg);
        break;
      case 'v':
        printf("Option -v (--verbose) selected with optional argument %s\n", optarg);
        break;
      case 'S':
        printf("Option -S (--stow) selected\n");
        break;
      case 'D':
        printf("Option -D (--delete) selected\n");
        break;
      case 'R':
        printf("Option -R (--restow) selected\n");
        break;
      case 'V':
        printf("Option -V (--version) selected\n");
        break;
      case 'h':
        printf("Option -h (--help) selected\n");
        break;
      case ':':
        printf("Option needs a value\n");
        break;
      case '?':
        printf("Unknown option: %c\n", optopt);
        break;
    }
  }


  // Extra arguments

  char* package_name;

  if (optind < argc) {
    printf("Package specified: %s\n", argv[optind]);
    package_name = argv[optind];
    optind++;
  }

  for (; optind < argc; optind++) {
    printf("Extra argument: %s\n", argv[optind]);
  }

  char* config = read_config(dir);
  printf(config);

  free(config);
  return 0;
  /*
  printf("You have entered %d arguments:\n", argc);

  for (int i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);
  }
  
  return 0;
  */
}



char* listdir(char* directory) {
  //char* res[] = "";

  struct dirent *de; // Pointer for directory entry
  
  // opendir() returns a pointer of type DIR
  DIR *dr = opendir(".");

  if (dr == NULL) { // opendir returns NULL if it couldn't open directory
    printf("Could not open directory");
    return 0;
  }

  while ((de = readdir(dr)) != NULL) {
    if (de->d_type != DT_DIR) {
      printf("File is not a directory: %s, skipping...\n", de->d_name);
      continue;
    }

    if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
      continue;
    }

    printf("%s ", de->d_name);
    printf("%d\n", de->d_type);

  }

  closedir(dr);
  return 0;
}
