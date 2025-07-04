#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include <pwd.h>
#include <sys/types.h>

#include <stdbool.h>

#include "modules/config_manager.h"


int main(int argc, char* argv[]);

int process_directory(char* directory);

void symlink_directory(char* source, char* dest);

void drop_privileges();

static struct option long_options[] = {
  {"no", no_argument, NULL, 'n'},
  {"dir", required_argument, NULL, 'd'},
  {"target", required_argument, NULL, 't'},
  {"verbose", optional_argument, NULL, 'v'},
  {"stow", no_argument, NULL, 'S'},
  {"delete", no_argument, NULL, 'D'},
  {"restow", no_argument, NULL, 'R'},

  // TODO: Finish implementations
  {"adopt", no_argument, NULL, 0},
  {"no-folding", no_argument, NULL, 0},
  {"ignore", required_argument, NULL, 0},
  {"defer", required_argument, NULL, 0},
  {"override", required_argument, NULL, 0},
  {"dotfiles", no_argument, NULL, 0},

  {"version", no_argument, NULL, 'V'},
  {"help", no_argument, NULL, 'h'},
  {NULL, 0, NULL, 0}
};

bool opt_no = false;
bool opt_dir = false;
bool opt_target = false;
bool opt_verbose = false;
bool opt_stow = false;
bool opt_delete = false;
bool opt_restow = false;


bool opt_adopt = false;
bool opt_no_folding = false;
bool opt_ignore = false;
bool opt_defer = false;
bool opt_override = false;
bool opt_dotfiles = false;

bool opt_version = false;
bool opt_help = false;


#endif // MAIN_H_
