#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "modules/config_manager.h"


int main(int argc, char* argv[]);

char* listdir(char* directory);

#endif // MAIN_H_
