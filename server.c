#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "struct_server.h"

#define PORT 8080

struct user users[MAX_LIST_SIZE];
struct account a1;

struct client client_array[MAX_CLIENTS];
bool clients[MAX_CLIENTS];

int sock_fd;
struct sockaddr_in server, client;

socklen_t len;
char buffer[BUFFER_SIZE_RECIEVE];
char response[BUFFER_SIZE_SEND];
int val_return;

void syserr(char *messsage){
    perror(messsage);
}

bool accept_client(){
    struct sockaddr_in new_client = { 0 };
    socklen_t length = sizeof(new_client);
    val_return = accept(sock_fd, (struct sockaddr*)&new_client, &length);
    if(val_return < 0){
        syserr("error of accept");
        return true;
    }
    printf("accept of client successful\n");
    client_array[0].fd = val_return;
    return false;
}

bool send_spend(struct user u1, int32_t amount){
    if(amount < 0){
        syserr("The amount must be positive");
        return true;
    }else{
        u1.balance -= amount/2;
        users[1].balance += amount/2;
        a1.total += amount;
    }
    return false;
}

bool send_refund(struct user u1, struct user u2, int32_t amount){
    if(amount < 0){
        syserr("The amount must be positive");
        return true;
    }else{
        u1.balance += amount/2;
        u2.balance -= amount/2;
        a1.total += amount;
    }
    return false;
}

int main(int argc, char *agrv[]){
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    int max = sock_fd;
    memset(clients, 1, sizeof(clients));

    /* USER */
    strcpy(users[0].name, "foozy");
    users[0].balance = 0;

    strcpy(users[1].name, "barry");
    users[1].balance = 0;
    
    /* ACCOUNT */
    strcpy(a1.title, "account 1");
    a1.list_user[0] = users[0];
    a1.list_user[1] = users[1];
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

    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) { // à retirer, pas sur un projet final
        syserr("setsockopt");
        return 1;
    }
   
    if(bind(sock_fd, (struct sockaddr*)&server, sizeof(server)) == -1){
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
        FD_ZERO (&set); // clean the set
        FD_SET(sock_fd, &set);
        
        for(int i=0; i<MAX_CLIENTS; i++){
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

        size_t nbread;
        client_array[0].in = fdopen(client_array[0].fd, "r");
        if(client_array[0].in == NULL){
            syserr("error of fdopen");
            return 1;
        }
        nbread = fread(buffer, sizeof(char), 1024, client_array[0].in);
        
        if(nbread < 1){
            syserr("error of fread");
            return 1;
        }
       
        if(strcmp(buffer, "state") == 0) {
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
        }else if(strcmp(buffer, users[0].name) == 0){
            char user_name[MAX_CLIENT_NAME_LENGTH];
            memcpy(&user_name, buffer, sizeof(user_name));
            printf("user_name: %s\n", user_name);
            int32_t amount = 0;
            memcpy(&amount, buffer, sizeof(amount));
            printf("amout: %d\n", amount);
        }else{
            printf("unknow command\n");
        }
    }
    fclose(client_array[0].file);
    fclose(client_array[0].in);
    fclose(client_array[0].out);
    close(sock_fd);
    return 0;
}

