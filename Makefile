TARGET=client server
CFLAGS= -g -Wall -Werror -Wconversion

all: client server

client: client.o

server: server.o

client.o: client.c 

server.o: server.c struct_server.h

clean:
	rm -f *.o

.PHONY: all