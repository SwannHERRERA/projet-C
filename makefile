gcc `pkg-config --cflags gtk+-3.0` -Wall -Wextra -o main main.c list_directory.c `pkg-config --libs gtk+-3.0` && ./main
