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
#define MAX_CLIENTS 10

struct user u1;
struct account a1;

struct client client_array[MAX_CLIENTS];
bool clients[MAX_CLIENTS];

int sock_fd;
struct sockaddr_in server, client;

socklen_t len;
char buffer[BUFFER_SIZE];

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

bool accept_client(){
    struct sockaddr_in csin = { 0 };
    socklen_t length = sizeof(csin);
    int val_return = accept(sock_fd, (struct sockaddr*)&csin, &length);
    if(val_return < 0){
        syserr("error of accept");
        return true;
    }
    printf("accept of client successful\n");
    client_array[0].fd = val_return;
    size_t nbread;
    client_array[0].file = fdopen(client_array[0].fd, "w+");
    nbread = fread(&val_return, sizeof(char), sizeof(buffer), client_array[0].file);
    if(nbread < -1){
        syserr("fread");
        return true;
    }
    printf("number i read : %ld\n", nbread); 
    return false;
}

int main(int argc, char* agrv[]){
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    int max = sock_fd;
    memset(clients, 1, sizeof(clients));

    /* USER */
    strcpy(u1.name, "toto");
    u1.balance = 0.0;

    /* ACCOUNT */
    strcpy(a1.title, "account 1");
    a1.list_user[0] = u1;
    a1.total = 0.0;

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

    // accept_sock = accept(sock_fd, (struct sockaddr*)&client, &len);
    // FILE* fdc = fdopen(accept_sock, "w+");

    
    //FD_SET(sock_fd, &client_fd);
   // FD_SET(accept_sock, &client_fd);

    while(true){
       /* fd_set client_fd;
        FD_ZERO(&client_fd);
        FD_SET(sock_fd, &client_fd);

        int nbSelect = select(FD_SETSIZE, &client_fd, NULL, NULL, NULL);
         
        if(nbSelect < 0){
            syserr("error of select");
            return 1;
        }
        */
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
        }else{
           // treate_client(set);
     /*     //  client_array.file = fdopen(client_array.fd, "w+");
            size_t nbread; 
           nbread = fread(&set, sizeof(buffer), 1, client_array->file);
            printf("number i read : %ld\n", nbread);
        */
        }

       /* size_t nbread; 
        nbread = fread(&set, sizeof(buffer), 1, client_array[0].file);
        printf("number i read : %ld\n", nbread);  */
        
        /* if(accept_sock < 0){
            syserr("error of connexion with client\n");
            return 1;
        } */
       // size_t nbread = fread(buffer, sizeof(buffer), 1, fdc);
       // printf("number i read : %ld\n", nbread);
        // connect_fd = accept(sock_fd, (struct sockaddr*)&client, &len);
        
       // fread(buffer, sizeof(buffer), 1, fdc);
        if(strcmp(buffer, "state") == 0) {
            memcpy(&buffer, &a1, sizeof(a1));
          //  fwrite(buffer, sizeof(buffer),1, fdc);
        }
    }
    fclose(client_array[0].file);
   // close(accept_sock);
    return 0;
}

