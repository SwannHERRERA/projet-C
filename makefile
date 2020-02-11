CC=gcc
CFLAGS= -Wall -Wextra -Werror
dev: build start
# main.c
build:
	$(CC) `pkg-config --cflags gtk+-3.0` $(CFLAGS) -o main.o main.c file_util.c param_file.c `pkg-config --libs gtk+-3.0` -export-dynamic
start:
	./main.o
