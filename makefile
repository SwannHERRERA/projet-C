CC=gcc
CFLAGS= -Wall -Wextra
dev: build start
build:
	$(CC) `pkg-config --cflags gtk+-3.0` $(CFLAGS) -o main.o main.c file_action.c `pkg-config --libs gtk+-3.0`
start:
	./main.o
