#include "main.h"

int main(int argc, char* argv[]) {

  int opt;

  char* dir = ".";

  while ((opt = getopt_long(argc, argv, ":nd:t:v::SDRVh", long_options, NULL)) != -1) {
    switch (opt) {
      case 'n':
        printf("Option -n (--no, --simulate) selected\n");
        opt_no = true;
        break;
      case 'd':
        printf("Option -d (--dir) selected with argument %s\n", optarg);
        dir = optarg;
        opt_dir = true; break;
      case 't':
        printf("Option -t (--target) selected with argument %s\n", optarg);
        opt_target = true;
        break;
      case 'v':
        printf("Option -v (--verbose) selected with optional argument %s\n", optarg);
        opt_verbose = true;
        break;
      case 'S':
        printf("Option -S (--stow) selected\n");
        opt_stow = true;
        break;
      case 'D':
        printf("Option -D (--delete) selected\n");
        opt_delete = true;
        break;
      case 'R':
        printf("Option -R (--restow) selected\n");
        opt_restow = true;
        break;
      case 'V':
        printf("Option -V (--version) selected\n");
        opt_version = true;
        break;
      case 'h':
        printf("Option -h (--help) selected\n");
        opt_help = true;
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
  printf("aksdlawldk laskd \n");

  char* package_dir = join_path(dir, package_name);

  process_directory(package_dir);

  return 0;
  /*
  printf("You have entered %d arguments:\n", argc);

  for (int i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);
  }
  
  return 0;
  */
}



int process_directory(char* directory) {

  struct dirent *de; // Pointer for directory entry
  
  // opendir() returns a pointer of type DIR
  DIR *dr = opendir(directory);

  if (dr == NULL) { // opendir returns NULL if it couldn't open directory
    printf("Could not open directory");
    return 0;
  }


  while ((de = readdir(dr)) != NULL) {
    /*
    if (de->d_type != DT_DIR) {
      printf("File is not a directory: %s, skipping...\n", de->d_name);
      continue;
    }
    */

    if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
      continue;
    }
    printf("1234\n");

    char* alias = parse_alias(de->d_name);

    if (!alias)
      continue;

    char* source_path = join_path(directory, de->d_name);
    char* dest_path = alias;

    struct dirent *pe;

    DIR* pr = opendir(source_path);

    if (dr == NULL) { // opendir returns NULL if it couldn't open directory
      printf("Could not open directory");
      return 0;
    }


    if (opt_verbose) {
      printf("LINK: %s => %s\n", de->d_name, dest_path);
    }


    if (opt_no) {
      // Simulate
      continue;
    }

    // Perform linking
    if (symlink(source_path, dest_path) == -1) {
      // Destination folder already exists, symlinking nested files individually
      symlink_directory(source_path, dest_path);
    }

    //printf("\n\n\n$HOME: %s\n", getenv("HOME"));
  }

  closedir(dr);
  return 0;
}


void symlink_directory(char* source, char* dest) {
  printf("Symlinking: %s => %s\n", source, dest);
  struct dirent *de; // Pointer for directory entry
  
  // opendir() returns a pointer of type DIR
  DIR *dr = opendir(source);

  if (dr == NULL) { // opendir returns NULL if it couldn't open directory
    printf("Could not open directory");
  }

  while ((de = readdir(dr)) != NULL) {
    if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
      continue;
    }

    if (symlink(source, dest) == -1) {
      if (de->d_type != DT_DIR) {
        printf("Error: Can't enter file %s - not a directory\n", join_path(source, de->d_name));
        return;
      }

      printf("Couldnt symlink\n");
      symlink_directory(join_path(source, de->d_name), join_path(dest, de->d_name));
    }
  }
}



