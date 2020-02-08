#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main (int argc, char** argv) {
    int result = mkdir("/home/swannherrera/Documents/test2/", 0777);
    printf("%d", result);
}