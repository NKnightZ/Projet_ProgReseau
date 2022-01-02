#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include "struct_server.h"

#define PORT 8080

void syserr(char *message);
bool accept_client();
bool reading_client();
bool send_client(FILE *f);
void send_spend(struct user *user_source, int32_t amount);
void send_refund(struct user *user_from, struct user *user_source, int32_t amount);