#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Params {
    char* display_hidden_file;
    char* sort_by;
    char* start_folder;
} Params;


char* get_param(char* key_search, FILE* pf);
Params* get_params(void);