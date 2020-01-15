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

/*
* Je veux un tableau de MY_FILE qui fait la taille du dossier
*/

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
    char* full_path;
    long path_max;
    size_t size;
    path_max = pathconf(".", _PC_PATH_MAX);
    if (path_max == -1) {
        size = 1024;
    } else if (path_max > 10240) {
        size = 10240;
    } else {
        size = path_max;
    }
    char dir[strlen(path)]; // convert cosnt char* to char*
    strcpy(dir, path);
    full_path = getcwd(dir, size);
    return full_path;
}

MY_FILE*
list_directory(const char* path) {
    u_int8_t i = 0;
    u_int16_t size_of_dir;
    char* actual_path;
    DIR *actual_directory;
    struct dirent *directory;
    struct stat statbuf;

    actual_path = get_full_path(path);
    printf("actual path : %s\n", actual_path);

    actual_directory = opendir(path);
    size_of_dir = count_nb_file_in_dir(path);
    MY_FILE* files = (MY_FILE*)malloc(sizeof(MY_FILE *) * size_of_dir);

    if (actual_directory && files) {
        while ((directory = readdir(actual_directory)) != NULL) {
            (files + i)->name = (char*)malloc(sizeof(char) * strlen(directory->d_name));
            (files + i)->name = strcpy(files[i].name, directory->d_name);
            // mettre la taille, isdir, date de modification
            if (stat(strcat(strcat(actual_path, "/"), directory->d_name), &statbuf) == -1) {
                perror("stat");
                exit(1);
            }
            if (S_ISDIR(statbuf.st_mode)) {
                files[i].is_dir = true;
            } else {
                files[i].is_dir = false;
            }
            (files + i)->last_change = statbuf.st_mtime;
            i += 1;
        }
        closedir(actual_directory);
    } else exit(1);
    return files;
}

int
main(int argc, char** argv) {
    if (argc < 2) {
        printf("Nombre d'argument incorrect\n");
        exit(1);
    }
    MY_FILE* files = list_directory(argv[1]);
    printf("%s\n", files[1].name);
    return EXIT_SUCCESS;
}