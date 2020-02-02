#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE* pf = fopen("./param.txt", "rb");
    
    fclose(pf);
}