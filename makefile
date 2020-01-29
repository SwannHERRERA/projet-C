CC=gcc
CFLAGS= -Wall -Wextra
dev: build start
# main.c
build:
	$(CC) `pkg-config --cflags gtk+-3.0` $(CFLAGS) -o main.o file_manager.c file_action.c `pkg-config --libs gtk+-3.0` -export-dynamic
start:
	./main.o
