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

struct user u1;
struct user u2;

struct account a1;

struct client client_array[MAX_CLIENTS];
bool clients[MAX_CLIENTS];

int sock_fd;
struct sockaddr_in server, client;

socklen_t len;
char buffer[BUFFER_SIZE];
char response[RESPONSE_SIZE];
int val_return;

void syserr(char *messsage){
    perror(messsage);
}

bool accept_client(){
    struct sockaddr_in new_client = {0};
    socklen_t length = sizeof(new_client);
    val_return = accept(sock_fd, (struct sockaddr *)&new_client, &length);
    if(val_return < 0){
        syserr("error of accept");
        return true;
    }
    printf("accept of client successful\n");
    for(int i = 0; i < MAX_CLIENTS; i++){
        client_array[i].fd = val_return;
    }
    return false;
}

bool reading_client(){
    size_t nbread;
    client_array[0].in = fdopen(client_array[0].fd, "r");
    if(client_array[0].in == NULL){
        syserr("error of fdopen");
        return true;
    }
    nbread = fread(buffer, sizeof(char), 1024, client_array[0].in);
    if(nbread < 1){
        syserr("error of fread");
        fclose(client_array[0].in);
        return true;
    }
    return false;
}

bool send_spend(struct user u1, int32_t amount){
    for(int i = 0; i < MAX_LIST_SIZE; i++){
        if(strcmp(a1.list_user[i].name, u1.name)){
            a1.list_user[i].balance -= (unsigned int)amount / 2;
        }else{
            a1.list_user[i].balance += (unsigned int)amount / 2;
        }
    }
    a1.total += amount;
    return false;
}

bool send_refund(struct user u1, struct user u2, int32_t amount){
    struct user user_temp;
    for(int i = 0; i < MAX_LIST_SIZE; i++){
        if(strcmp(a1.list_user[i].name, u1.name) == 0){     
            user_temp = a1.list_user[i]; 
            for(int i = 0; i < MAX_LIST_SIZE; i++){
                if(strcmp(a1.list_user[i].name, u2.name) == 0){
                    user_temp.balance += (unsigned int)amount / 2;
                    a1.list_user[i].balance -= (unsigned int)amount / 2;
                }     
            } 
            a1.list_user[i] = user_temp; 
        } 
    }
    a1.total += amount;
    return false;
}

int main(int argc, char *agrv[]){
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    int max = sock_fd;
    memset(clients, 1, sizeof(clients));
    char parsed[1024][1024];

    /* USER */
    strcpy(u1.name, "foozy");
    u1.balance = 0;

    strcpy(u2.name, "barry");
    u2.balance = 0;

    /* ACCOUNT */
    strcpy(a1.title, "account 1");
    a1.list_user[0] = u1;
    a1.list_user[1] = u2;
    a1.total = 0;

    if(sock_fd == -1){
        syserr("Error with creation of socket\n");
        return 1;
    }else{
        printf("Socket creation successful\n");
    }

    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);
    server.sin_family = AF_INET;

    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){ // à retirer, pas sur un projet final
        syserr("setsockopt");
        return 1;
    }

    if(bind(sock_fd, (struct sockaddr *)&server, sizeof(server)) == -1){
        syserr("Error with binding of socket\n");
        return 1;
    }else{
        printf("Bind of socket successful\n");
    }

    len = sizeof(client);

    if(listen(sock_fd, 10) == 0){
        printf("[+]Listening....\n");
    }else{
        syserr("[-]Error in binding.\n");
        return 1;
    }

    while(true){
        fd_set set;
        FD_ZERO(&set); // clean the set
        FD_SET(sock_fd, &set);

        for(int i = 0; i < MAX_CLIENTS; i++){
            if(clients[i]){
                FD_SET(client_array[i].fd, &set);
            }
        }

        int nbSelect = select(max + 1, &set, NULL, NULL, NULL);

        if(nbSelect < 0){
            syserr("error of select");
            return 1;
        }

        if(FD_ISSET(sock_fd, &set)){
            accept_client();
        }

        if(reading_client() == false){
            if(strcmp(buffer, "state") == 0){
                client_array[0].out = fdopen(client_array[0].fd, "w");
                if(client_array[0].out == NULL){
                    syserr("error of fdopen");
                    return 1;
                }
                printf("sending the state of account...\n");
                memcpy(response, &a1, sizeof(a1));
                size_t nb_write = fwrite(response, sizeof(char), sizeof(response), client_array[0].out);
                if(nb_write < 0){
                    syserr("error of fwrite");
                    return 1;
                }
                if(fflush(client_array[0].out)){
                    syserr("error of fflush");
                }
            }else{
                int j = 0;
                int k = 0;
                char temp[BUFFER_SIZE];
                for(size_t i = 0; i < strlen(buffer); i++){
                    if(isspace(buffer[i])){
                        temp[i] = '\0';
                        strcpy(parsed[j], temp);
                        for (size_t k = 0; k < BUFFER_SIZE; k++){
                            temp[k] = '\0';
                        }
                        k =- 1;
                        j++;
                    }
                    temp[k] = buffer[i];
                    if(buffer[i+1] == '\0'){
                        buffer[i+1] = '\0';
                        strcpy(parsed[j], temp);
                    }
                    k++;
                }
                bool find = false;
                for(int i = 0; i < MAX_LIST_SIZE; i++){
                    if(strcmp(parsed[0], a1.list_user[i].name) == 0){
                        struct user temp = a1.list_user[i];
                        int amount;
                        if(strcmp(parsed[1], "spend") == 0){   
                            amount = atoi(parsed[2]);
                            if(amount > 0){
                                send_spend(a1.list_user[i], amount);
                                memcpy(response, "Operation sucessful !", strlen("Operation sucessful !"));
                                printf("%s\n", response);
                            }else{
                                memcpy(response, "Operation failed: the amount cannot be negative.", strlen("Operation failed: the amount cannot be negative."));
                                printf("%s\n", response);
                            }
                            
                        }else if(strcmp(parsed[1], "refund") == 0){
                            amount = atoi(parsed[3]);
                            for(int i = 0; i < MAX_LIST_SIZE; i++){
                                if(strcmp(parsed[2], a1.list_user[i].name) == 0){
                                    if(amount > 0){
                                        send_refund(temp, a1.list_user[i], amount);
                                        memcpy(response, "Operation sucessful !", strlen("Operation sucessful !"));
                                        printf("%s\n", response);
                                    }else{
                                        memcpy(response, "Operation failed: the amount cannot be negative.", strlen("Operation failed: the amount cannot be negative."));
                                        printf("%s\n", response);
                                    }
                                }
                            } 
                        }
                        client_array[0].out = fdopen(client_array[0].fd, "w");
                            size_t nb_write = fwrite(response, sizeof(char), sizeof(response), client_array[0].out); // sending the response to the client
                            if(nb_write < 0){
                                syserr("error of fwrite");
                                return 1;
                            }
                            if(fflush(client_array[0].out)){
                                syserr("error of fflush");
                            }
                            for (size_t i = 0; i < RESPONSE_SIZE; i++){
                                response[i] = '\0';
                            }
                            find = true;
                            break;
                    }
                }
                if(find == false){
                    printf("user not found\n");
                    memcpy(response, "user not found", strlen("user not found"));
                    client_array[0].out = fdopen(client_array[0].fd, "w");
                    size_t nb_write = fwrite(response, sizeof(char), sizeof(response), client_array[0].out);
                    if(nb_write < 0){
                        syserr("error of fwrite");
                        return 1;
                    }
                    if(fflush(client_array[0].out)){
                        syserr("error of fflush");
                    }
                    for (size_t i = 0; i < RESPONSE_SIZE; i++){
                        response[i] = '\0';
                    }
                }
            }
        }else{
            printf("unknown command\n");
        }
    }
    fclose(client_array[0].in);
    fclose(client_array[0].out);
    close(sock_fd);
    return 0;
}
