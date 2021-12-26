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

char buffer[BUFFER_SIZE];
char response[RESPONSE_SIZE];

void syserr(char *message) {
    perror(message);
}

void display_state_info(){
    printf("\n");
    printf("account title: %s\n", a1.title);
    int nb_user = 0;
    for(int i = 0; i < MAX_LIST_SIZE; i++){
        if(strcmp(a1.list_user[i].name, "\0")){
            nb_user++;
        }
    }
    for(int i = 0; i < nb_user; i++){
        printf("\n");
        printf("--- user %d ---\n", i+1);
        printf("name: %s\n", a1.list_user[i].name);
        printf("balance: %d\n", a1.list_user[i].balance);
    }
    printf("total: %d\n", a1.total);
}

// send_refund(from, source, amount)

// send_spend(user, amount)

int main(int argc, char *argv[]){
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
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

    if(argc > 1 && argc <= 5){
        if(argc == 2 && strcmp(argv[1], "state") == 0){
            strcpy(buffer, argv[1]);
            size_t nb_write = fwrite(buffer, sizeof(char), sizeof(buffer), fdc);
            if(nb_write < 0){
                syserr("error of fwrite");
                return 1;
            }
            if(fflush(fdc)){
                syserr("error of fflush");
            }
            size_t nb_read = fread(response, sizeof(char), sizeof(buffer), fdc);
            if(nb_read < 0){
                syserr("error of fread");
                return 1;
            }else{
                memcpy(&a1, response, sizeof(a1));
            }
            display_state_info();
        }else if(strcmp(argv[2], "spend") == 0){
            strcpy(buffer, argv[1]);
            strcat(buffer, " ");
            strcat(buffer, argv[2]);
            strcat(buffer, " ");
            strcat(buffer, argv[3]);
            size_t nb_write = fwrite(buffer, sizeof(char), sizeof(buffer), fdc);
            if(nb_write < 0){
                syserr("error of fwrite");
                return 1;
            }
            if(fflush(fdc)){
                syserr("error of fflush");
            }
            size_t nb_read = fread(response, sizeof(char), sizeof(response), fdc);
            if(nb_read < 0){
                syserr("error of fread");
                return 1;
            }
            printf("%s\n", response);
        }else{
            printf("unknown command\n");
        }
    }else{
        printf("too much arguments, nothing will be send\n");
    }
    fclose(fdc);
    close(sock_fd);
    return 0;
}