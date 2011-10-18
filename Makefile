
CC=gcc

all: fullscreenize.c
	$(CC) $? -Wall -lX11 -o fullscreenize
