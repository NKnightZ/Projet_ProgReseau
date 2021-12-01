#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "struct_server.h"

#define PORT 8080

void syserr(char* messsage){
    perror(messsage);
}

void putUInt32(uint32_t **ptr, uint32_t x){
    **ptr = x;
    *ptr += 1;
}

bool serialise(char *buff, int buff_len, struct account *acc){
   /* *buff = // ecrire 1 octet
    buff+=1;
    *(uint32_t*)buff =  // ecrire 1 uint32_t
    buff+=sizeof(int32_t); */
    putUInt32((uint32_t**)&buff, 3);
    return true;
}

int main(int argc, char* agrv[]){
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
   // FILE* fd = fdopen(sock_fd, "w");
    int yes = 1;

    /* USER */
    struct user u1; 
    strcpy(u1.name, "toto");
    u1.balance = 0.0;

    /* ACCOUNT */
    struct account a1;
    strcpy(a1.title, "account 1");
    a1.list_user[0] = u1;
    a1.total = 0.0;

    if(sock_fd == -1){
        syserr("Error with creation of socket\n");
        return 1;
    }else{
        printf("Socket creation successful\n");
    }

    struct sockaddr_in server, client;

    server.sin_addr.s_addr = htonl(INADDR_ANY); 
    server.sin_port = htons(PORT); 
    server.sin_family = AF_INET;

    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        syserr("setsockopt");
        return 1;
    }
   
    if(bind(sock_fd, (struct sockaddr*)&server, sizeof(server)) == -1){
        syserr("Error with binding of socket\n");
        return 1;
    }else{
        printf("Bind of socket successful\n");
    }
    
    socklen_t len = sizeof(client);
    
    if(listen(sock_fd, 10) == 0){
        printf("[+]Listening....\n");
    }else{
        syserr("[-]Error in binding.\n");
        return 1;
    }

    int connect_fd;

    connect_fd = accept(sock_fd, (struct sockaddr*)&client, &len);
    FILE* fdc = fdopen(connect_fd, "w");

    while(1){
        if(connect_fd < 0){
            syserr("error of connexion with client\n");
            return 1;
        }else{
            fwrite(a1.list_user->name, sizeof(char), 1, fdc);
            connect_fd = accept(sock_fd, (struct sockaddr*)&client, &len);
        }
    }
    close(connect_fd);
    return 0;
}

