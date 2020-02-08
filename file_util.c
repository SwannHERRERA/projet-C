#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_util.h"

char* get_full_path(const char* path) {
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

void create_file(char* path) {
    struct stat f;
    if (stat(path, &f) == -1) {
        FILE* new_file = fopen(path, "w");
        fclose(new_file);
    } else {
        printf("Erreur le fichier existe déjà\n");
        exit(EXIT_FAILURE);
    }
}

void rename_file(char* path, char* new_name) {
    if (rename(path, new_name) == -1) {
        printf("error");
        exit(EXIT_FAILURE);
    }
}

void delete_file(char* path) {
    struct stat f;
    char file_exist = stat(path, &f);
    if (file_exist == -1) {
        printf("Erreur le fichier n'existe\n");
        perror("stat");
        exit(EXIT_FAILURE);
    } else if (file_exist == 0) {
        unlink(path);
    } else {
        printf("Unexpected error");
        exit(EXIT_FAILURE);
    }
}

short remove_directory(const char *path) {
    DIR *directory = opendir(path);
    size_t path_len = strlen(path);
    short r = -1;
    if (directory) {
        r = 0;
        struct dirent* file;
        while (!r && (file=readdir(directory))) {
            printf("%s\n",file->d_name);
            short r2 = -1;
            char *buf;
            size_t len;

            /* Skip "." and ".." */
            if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) {
                continue;
            }
            len = path_len + strlen(file->d_name) + 2;
            buf = malloc(sizeof(char) * len);
            snprintf(buf, len, "%s/%s", path, file->d_name);
            if (buf) {
                struct stat statbuf;
                if (!stat(buf, &statbuf)) {
                    if (S_ISDIR(statbuf.st_mode)) {
                        r2 = remove_directory(buf);
                    }
                    else {
                        r2 = unlink(buf);
                    }
                }
                free(buf);
            }
            r = r2;
        }
        closedir(directory);
    }
    if (!r) {
        r = rmdir(path);
    }
    return r;
}

// int main (int argc, char** argv) {
//     if (argc < 2) {
//         printf("Nombre d'argument incorrect\n");
//         return EXIT_FAILURE;
//     }
//     unsigned int user_entry;
//     char* full_path = get_full_path(argv[1]);

//     printf("Que voulez vous faire ?\n");
//     printf("Tapper 1 pour crée un fichier\n");
//     printf("tapper 2 pour supprimé un fichier\n");
//     printf("Tapper 3 pour renomé le fichier en RARRAR.AR\n");
//     printf("Tapper 4 pour supprimé un folder\n");
//     scanf("%u", &user_entry);
//     switch (user_entry)
//     {
//     case 1:
//         full_path = (char*)malloc(sizeof(char) * (strlen(full_path) + 1 + strlen(argv[1])));
//         strcat(strcat(strcpy(full_path, get_full_path(argv[1])), "/"), argv[1]);
//         create_file(full_path);
//         break;
//     case 2:
//         full_path = (char*)malloc(sizeof(char) * (strlen(full_path) + 1 + strlen(argv[1])));
//         strcat(strcat(strcpy(full_path, get_full_path(argv[1])), "/"), argv[1]);
//         delete_file(full_path);
//         break;
//     case 3:
//         full_path = (char*)malloc(sizeof(char) * (strlen(full_path) + 1 + strlen(argv[1])));
//         strcat(strcat(strcpy(full_path, get_full_path(argv[1])), "/"), argv[1]);
//         rename_file(full_path);
//         break;

//     case 4:
//         full_path = (char*)malloc(sizeof(char) * (strlen(full_path) + 1 + strlen(argv[1])));
//         strcat(strcat(strcpy(full_path, get_full_path(argv[1])), "/"), argv[1]);
//         remove_directory(full_path);
//         break;

//     default:
//         break;
//     }

//     return EXIT_SUCCESS;
// }

u_int16_t count_nb_file_in_dir(const char* path) {
    struct dirent* entry;
    DIR * directory;
    u_int16_t file_count = 0;
    directory = opendir(path);
    while ((entry = readdir(directory)) != NULL) {
        if (strchr(entry->d_name, '.') == NULL) {
            file_count += 1;
        }
    }
    closedir(directory);
    return file_count;
}

MY_FILE* list_directory(const char* path) {
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
            if (strchr(directory->d_name, '.') != NULL) {
                continue;
            }
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
void create_folder(char* path, char* name)
{
    char* path_of_new_folder;
    path_of_new_folder = (char*)malloc(
        sizeof(char) * (strlen(path) + strlen(name) + 2));
    strcpy(path_of_new_folder, path);
    strcat(path_of_new_folder, "/");
    strcat(path_of_new_folder, name);
    struct stat st;
    if (path != NULL) {
        if (stat(path, &st) == 0) {
            if (0 != mkdir(path_of_new_folder, 0777)) {
                printf("error: mkdir('%s')\n", path);
                perror("mkdir");
                exit(EXIT_FAILURE);
            }
        }
    }
    free(path_of_new_folder);
}