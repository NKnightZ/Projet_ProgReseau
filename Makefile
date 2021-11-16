TARGET=client server 
CC=gcc
CFLAGS= -g -Wall 

all:
	@echo "Compiling client..."
	$(CC) client.c -o client
	@echo "Compiling server..."
	$(CC) server.c -o server