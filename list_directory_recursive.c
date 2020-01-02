/*
Author: Swann HERRERA
Date: 25/11/2019
Objectif: List all file in a folder
*/
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_regular_file(const char *path);
void list_directory_content(const char *path);

int
main(int argc, char** argv) {
    if (argc < 2) {
        printf("Nombre d'argument incorrect\n");
        exit(1);
    }
    list_directory_content(argv[1]);
    return(0);
}

int
is_regular_file(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void 
list_directory_content(const char *path) {
    DIR *actual_directory;
    struct dirent *directory;
    actual_directory = opendir(path);
    if (actual_directory) {
        while ((directory = readdir(actual_directory)) != NULL) {
            printf("%s\n", directory->d_name);
            if (!is_regular_file(directory->d_name) && strcmp(directory->d_name, ".") == 0 && strcmp(directory->d_name, "..") == 0) {
                printf("\n");
                list_directory_content(directory->d_name);
            }
        }
        closedir(actual_directory);
    }
}

// https://stackoverflow.com/questions/8436841/how-to-recursively-list-directories-in-c-on-linux NOT BAD