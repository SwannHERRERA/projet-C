/*
Author: Swann HERRERA
Date: 19/01/2020
Objectif: List all file in a folder
*/
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define false  0
#define true   1
// if(S_ISDIR(statbuf.st_mode))

typedef char bool;
typedef struct MY_FILE {
    off_t size;
    char* name;
    bool is_dir;
    time_t last_change;
} MY_FILE;

MY_FILE* list_directory(const char* path);
char* get_full_path (const char* path);
u_int16_t count_nb_file_in_dir(const char* path);
