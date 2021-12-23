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

struct account a1;
struct user users[MAX_LIST_SIZE];

void syserr(char *message) {
    perror(message);
}

// send_refund(from, source, amount)

// send_spend(user, amount)

int main(int argc, char *argv[]){
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[BUFFER_SIZE_SEND];
    char response[BUFFER_SIZE_SEND];

    char response_user_name[BUFFER_SIZE_SEND];
    char response_action[BUFFER_SIZE_SEND];
    char response_amount[BUFFER_SIZE_SEND];
    
    if(sock_fd == -1){
        syserr("Error with creation of socket\n");
        return 1;
    }else{
        printf("Socket creation successful\n");
    }

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT); 
    server.sin_family = AF_INET;
    
    socklen_t len = sizeof(server);

    int connect_return;
    connect_return = connect(sock_fd, (struct sockaddr*) &server, len);
    
    if(connect_return < 0){
        syserr("Error of connection with the server\n");
        return 1;
    }else{
        printf("Connection successful !\n");
    }
    
    FILE* fdc = fdopen(sock_fd, "w+");

    if(fdc == NULL){
        syserr("error of fdopen");
        return 1;
    }

    if(argc > 1){
        if(strcmp(argv[1], "state") == 0){
            strcpy(response, argv[1]);
            size_t nb_write = fwrite(response, sizeof(char), 1024, fdc);
            if(nb_write < 0){
                syserr("error of fwrite");
                return 1;
            }
            if(fflush(fdc)){
                syserr("error of fflush");
            }
            size_t nb_read = fread(buffer, sizeof(char), sizeof(buffer), fdc);
            if(nb_read < 0){
                syserr("error of fread");
                return 1;
            }else{
                memcpy(&a1, buffer, sizeof(a1));
            }
            printf("account title: %s\n", a1.title);
            printf("--- user 1 ---\n");
            printf("user's name: %s\n", a1.list_user[0].name);
            printf("balance: %d\n", a1.list_user[0].balance);
            printf("--- user 2 ---\n");
            printf("user's name: %s\n", a1.list_user[1].name);
            printf("balance: %d\n", a1.list_user[1].balance);
            printf("total: %d\n", a1.total);
        }else if(strcmp(argv[1], users[0].name) == 0){
            if(strcmp(argv[2], "spend") == 0){
                printf("depense\n");
                strcpy(response_user_name, argv[1]);
                strcpy(response_action, argv[2]);
                strcpy(response_amount, argv[3]);
                size_t nb_write = fwrite(response_user_name, sizeof(char), 2048, fdc);
                printf("number i write: %ld\n", nb_write);
                if(fflush(fdc)){
                    syserr("error of fflush");
                }
                size_t nbread = fread(buffer, sizeof(char), sizeof(buffer), fdc);
                printf("number i read: %ld\n", nbread);
                memcpy(&a1, buffer, sizeof(a1));
                printf("user's name: %s\n", a1.list_user[0].name);
                printf("balance: %d\n", a1.list_user[0].balance);
                printf("--- user 2 ---\n");
                printf("user's name: %s\n", a1.list_user[1].name);
                printf("balance: %d\n", a1.list_user[1].balance);
                printf("total: %d\n", a1.total);
            }
        }else{
            strcpy(response, argv[1]);
            size_t nb_write = fwrite(response, sizeof(char), 1024, fdc);
            if(nb_write < 0){
                syserr("error of fwrite");
                return 1;
            }
            if(fflush(fdc)){
                syserr("error of fflush");
            }
            printf("unknown command\n");
            fclose(fdc);
        }
    }
    close(sock_fd);
    return 0;
}