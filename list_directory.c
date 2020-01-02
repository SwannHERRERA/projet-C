/*
Author: Swann HERRERA
Date: 24/11/2019
Objectif: List all file in a folder
*/
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char** argv) {
    if (argc < 2) {
        printf("Nombre d'argument incorrect\n");
        exit(1);
    }

    DIR *actual_directory;
    struct dirent *directory;
    actual_directory = opendir(argv[1]);
    if (actual_directory) {
        while ((directory = readdir(actual_directory)) != NULL) {
            printf("%s\n", directory->d_name);
        }
        closedir(actual_directory);
    }
    return(0);
}