TARGET=tricount server
CFLAGS= -g -Wall -Werror -Wconversion

all: tricount server

tricount: tricount.o

server: server.o

tricount.o: tricount.c 

server.o: server.c struct_server.h

clean:
	rm -f *.o

.PHONY: all