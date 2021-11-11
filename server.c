#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "struct.h"

#define PORT 8888

/*Server :
    Créer le socket
    Assigner les adresses
    bind
    accepter les clients > fork
    Communication
*/

struct var{
    double balance;
    double total;
    char* utility; 
};

void syserr(char* messsage){
    perror(messsage);
}

int main(int argc, char* agrv[]){
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[1024];
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

    int bind_fd = bind(sock_fd, (struct sockaddr*)&server, sizeof(server));
    
    if(bind_fd == -1){
        syserr("Error with binding of socket\n");
        return 1;
    }else{
        printf("Bind of socket successful\n");
    }
    
    socklen_t len = sizeof(client);
    
    if(listen(sock_fd, 10) == 0) {
        printf("[+]Listening....\n");
    } else {
        syserr("[-]Error in binding.\n");
        return 1;
    }

    int connect_fd;
    connect_fd = accept(sock_fd, (struct sockaddr*)&client, &len);
    if(connect_fd < 0) {
        syserr("error of connexion with client\n");
        return 1;
    }
    printf("connexion with client successful\n");
    int r = recv(connect_fd, buffer, 1024, MSG_WAITALL);
    printf("%d", r);
    int nb; 
    printf("j'ai obtenu : %s\n", buffer);
    int s;

    while(r > 0){
        //printf("%d", r);
        /* if(strcmp(buffer, "!exit") == 0) {
            printf("Disconnected");
            break;
        } else {
           // printf("Return error", buffer);
            strncpy(buffer, "Undefined command", 1024);
            send(connect_fd, buffer, strlen(buffer), 0);
            bzero(buffer, sizeof(buffer));
        }*/
        if(!strcmp(buffer,"exit")){
            printf("exit...\n");
            break;
        }else{
            strncpy(buffer, "Undefined command", 1024);
            send(connect_fd, buffer, strlen(buffer), 0);
            bzero(buffer, sizeof(buffer));
        }
       /* printf("j'ai obtenu : %s\n", buffer);
        nb = read(STDIN_FILENO, &buffer, sizeof(buffer));
        buffer[strcspn(buffer, "\n")] = 0;
        s = send(sock_fd, buffer, nb, 0);
        r = recv(connect_fd, buffer, 1024, 0); */
    }
    return 0;
}

