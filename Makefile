TARGET=tricount server
CFLAGS= -g -Wall -Werror -Wconversion

all: tricount server

tricount: tricount.o
server: server.o

tricount.o: tricount.c tricount.h struct_server.h
server.o: server.c server.h struct_server.h

clean:
	rm -f *.o

.PHONY: all