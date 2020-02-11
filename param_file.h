#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char bool;
typedef struct Params {
    bool display_hidden_file;
    char* sort_by;
    char* start_folder;
    char* ui_file;
} Params;


char* get_param(char* key_search, FILE* pf);
Params* get_params(void);