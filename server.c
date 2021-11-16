#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "struct.h"

#define PORT 8080

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
    char log[10];
    int yes = 1;
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

    int bind_fd = bind(sock_fd, (struct sockaddr*)&server, sizeof(server));
    
    if(bind_fd == -1){
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
    int nbClient = 0;
    
    //int r = recv(connect_fd, buffer, 1024, MSG_WAITALL);
    //printf("%d", r);
   // int nb; 
  //  printf("j'ai obtenu : %s\n", buffer);
   // int s;
   // pid_t childpid;
    
    connect_fd = accept(sock_fd, (struct sockaddr*)&client, &len);
    while(1){
        if(connect_fd < 0){
            syserr("error of connexion with client\n");
            return 1;
        }else{
        //   printf("connexion with client successful\n");
            nbClient++;
            printf("number of client connected: %d\n", nbClient);
        }
        recv(connect_fd, buffer, strlen(buffer)+1, 0);
        //printf("%d", r);
        if(strcmp(buffer, "exit") == 0) {
            printf("A client has disconnected\n");
            nbClient--;
            printf("number of client connected: %d\n", nbClient);
            connect_fd = accept(sock_fd, (struct sockaddr*)&client, &len);
        }
       /* printf("j'ai obtenu : %s\n", buffer);
        nb = read(STDIN_FILENO, &buffer, sizeof(buffer));
        buffer[strcspn(buffer, "\n")] = 0;
        s = send(sock_fd, buffer, nb, 0);
        r = recv(connect_fd, buffer, 1024, 0); */
       /* if((childpid = fork()) == 0){
			close(sock_fd);
        } */
    }
    close(connect_fd);
    return 0;
}

