#define false  0
#define true   1
// if(S_ISDIR(statbuf.st_mode))

#include <sys/types.h>

typedef char bool;

typedef struct MY_FILE {
    off_t size;
    char* name;
    bool is_dir;
    time_t last_change;
} MY_FILE;

u_int16_t count_nb_file_in_dir(const char* path);

char* get_full_path (const char* path);

MY_FILE* list_directory(const char* path);

void create_file(char* path);

void rename_file(char* path);

void delete_file(char* path);

short remove_directory(const char *path);