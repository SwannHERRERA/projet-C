#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct node {
    char*  file_name;
    struct node* next;
} linked_list;

void add(int linked_list, char* file_name);

int
main (int argc, char **argv) {
    if (argc < 2) {
        printf("Nombre d'argument incorrect\n");
        exit(1);
    }

    linked_list * head = NULL;
    head = (linked_list *) malloc(sizeof(linked_list));
    if (head == NULL) {
        return EXIT_FAILURE;
    }

    head->file_name = ".";
    head->next = NULL;
    DIR *actual_directory;
    struct dirent *directory;
    actual_directory = opendir(argv[1]);
    if (actual_directory) {
        while ((directory = readdir(actual_directory)) != NULL) {
           // add(&struc, directory->d_name);
        }
        closedir(actual_directory);
    }
    return EXIT_SUCCESS;
}

void push(linked_list * head, char* file_name) {
    linked_list * current = head;
    while (current->next != NULL) {
        current = current->next;
    }
}