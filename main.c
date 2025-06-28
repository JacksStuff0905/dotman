#include <stdio.h>
#include <dirent.h>

int main(int argc, char* argv[]) {

  struct dirent *de; // Pointer for directory entry
  
  // opendir() returns a pointer of type DIR
  DIR *dr = opendir(".");

  if (dr == NULL) { // opendir returns NULL if it couldn't open directory
    printf("Could not open directory");
    return 0;
  }

  while ((de = readdir(dr)) != NULL)
    printf("%s\n", de->d_name);

  closedir(dr);
  return 0;


  /*
  printf("You have entered %d arguments:\n", argc);

  for (int i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);
  }
  
  return 0;
  */
}
