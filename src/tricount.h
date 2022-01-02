#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "struct_server.h"

#define PORT 8080

void syserr(char *message);
void display_state_info();
bool send_and_recieve(FILE *file);