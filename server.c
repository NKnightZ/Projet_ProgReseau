#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "struct_server.h"

#define PORT 8080
#define BUFFER_SIZE 1024

void syserr(char *messsage){
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
    int yes = 1;
    char buffer[BUFFER_SIZE];

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
    FILE* fdc = fdopen(connect_fd, "w+");

    fd_set read_fd;
    FD_ZERO(&read_fd);
    FD_SET(sock_fd, &read_fd);
    FD_SET(connect_fd, &read_fd);

    while(true){
        int nbSelect = select(FD_SETSIZE, &read_fd, NULL, NULL, NULL);
        
        if(nbSelect < 0){
            syserr("error of select");
            return 1;
        }

        if(FD_ISSET(sock_fd, &read_fd)){
            int client2;
            socklen_t len = sizeof(client2);

            client2 = accept(sock_fd, (struct sockaddr*)&client, &len);
            FD_SET(client2, &read_fd);
            size_t nbread; 
            nbread = fread(buffer, sizeof(buffer), 1, fdc);
            printf("number i read : %ld\n", nbread);
        }
        if(connect_fd < 0){
            syserr("error of connexion with client\n");
            return 1;
        }
        size_t nbread = fread(buffer, sizeof(buffer), 1, fdc);
        printf("number i read : %ld\n", nbread);
        // connect_fd = accept(sock_fd, (struct sockaddr*)&client, &len);
        
        fread(buffer, sizeof(buffer), 1, fdc);
        if(strcmp(buffer, "state") == 0) {
            memcpy(&buffer, &a1, sizeof(a1));
            fwrite(buffer, sizeof(buffer),1, fdc);
        }
    }
    close(connect_fd);
    return 0;
}

