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
#include <string.h>

int
main(int argc, char** argv) {
    if (argc < 2) {
        printf("Nombre d'argument incorrect\n");
        exit(1);
    }
    char** file_names = (char**)malloc(sizeof(char *) * 50);
    u_int8_t i, j = 0;
    DIR *actual_directory;
    struct dirent *directory;

    actual_directory = opendir(argv[1]);
    if (actual_directory && file_names) {
        while ((directory = readdir(actual_directory)) != NULL) {
            file_names[i] = (char*)malloc(sizeof(char) * strlen(directory->d_name));
            strcpy(file_names[i], directory->d_name);
            i += 1;
        }
        closedir(actual_directory);
    }
    while (j < 50 && file_names[j] != NULL) {
        printf("%s\n", file_names[j]);
        j += 1;
    }
    return EXIT_SUCCESS;
}