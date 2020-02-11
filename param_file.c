#include "param_file.h"

char* get_param(char* key_search, FILE* pf) {
    char buffer[512];
    char* result;
    char tmp[100];
    while(fgets(buffer, 512, pf) != NULL) {
		if((strstr(buffer, key_search)) != NULL) {
            fscanf(pf, "%100[^\n]", tmp);
            result = (char*)malloc(sizeof(char) * (strlen(strtok(tmp, "\n")) + 1));
            result = strtok(tmp, "\n");
            return result;
		}
	}
    return NULL;
}

Params* get_params(void) {
    FILE* pf = fopen("./param.txt", "rb");
    char tmp[100];
    Params* init;
    init = (Params*)malloc(sizeof(Params));
    
    strcpy(tmp, get_param("display hidden file", pf));
    init->display_hidden_file = (char*)malloc(sizeof(char) * strlen(tmp) + 1);
    strcpy(init->display_hidden_file, tmp);

    strcpy(tmp, get_param("sort by", pf));
    init->sort_by = (char*)malloc(sizeof(char) * strlen(tmp) + 1);
    strcpy(init->sort_by, tmp);


    strcpy(tmp, get_param("start folder", pf));
    init->start_folder = (char*)malloc(sizeof(char) * strlen(tmp) + 1);
    strcpy(init->start_folder, tmp);
    
    fclose(pf);

    return  init;
}