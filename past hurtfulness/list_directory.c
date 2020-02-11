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

u_int16_t
count_nb_file_in_dir(const char* path) {
    struct dirent * entry;
    DIR * directory;
    u_int16_t file_count = 0;
    directory = opendir(path);
    while ((entry = readdir(directory)) != NULL) {
        file_count += 1;
    }
    return file_count;
}
char*
get_full_path (const char* path) {
    long path_max;
    char* full_path;
    size_t size;
    path_max = pathconf(path, _PC_PATH_MAX);
    if (path_max == -1) {
        size = 1024;
    } else if (path_max > 10240) {
        size = 10240;
    } else {
        size = path_max;
    }
    // convert const char* to char*
    full_path = (char*)malloc(sizeof(char) * (size + 1));
    getcwd(full_path, size);
    return full_path;
}

MY_FILE*
list_directory(const char* path) {
    u_int8_t i = 0;
    // u_int8_t j;
    u_int16_t size_of_dir;
    char* path_of_file;
    DIR *actual_directory;
    struct dirent *directory;
    struct stat statbuf;

    actual_directory = opendir(path);
    size_of_dir = count_nb_file_in_dir(path);
    MY_FILE* files = (MY_FILE*)malloc(sizeof(MY_FILE) * (size_of_dir + 1));

    if (actual_directory && files) {
        while ((directory = readdir(actual_directory)) != NULL) {
            (files + i)->name = (char*)malloc(sizeof(char) * (strlen(directory->d_name) + 1));
            (files + i)->name = strcpy(files[i].name, directory->d_name);
            // mettre la taille, isdir, date de modification
            path_of_file = (char*)malloc(sizeof(char) * (strlen(path) + strlen(directory->d_name) + 2));
            // 2 = "/" & '\0'
            strcat(strcat(strcpy(path_of_file, path), "/"), directory->d_name);
            if (stat(path_of_file, &statbuf) == -1) {
                perror("stat");
                exit(EXIT_FAILURE);
            }
            if (S_ISDIR(statbuf.st_mode)) {
                files[i].is_dir = true;
            } else {
                files[i].is_dir = false;
            }
            (files + i)->last_change = statbuf.st_mtime;
            free(path_of_file);
            i += 1;
        }
        closedir(actual_directory);
    } else exit(EXIT_FAILURE);
    return files;
}
