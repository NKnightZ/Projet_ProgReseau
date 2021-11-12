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

void init_account(struct account a1){
    printf("To start you must to create a account\n");
    struct user u1;
    printf("enter your name: ");
    scanf("%s", u1.name);
    /* if(nb <= 0){
        printf("sorry your name it's invalid, please enter your name again: ");
    } */
    u1.balance = 0.0;
    printf("Hello %s\n", u1.name);
    printf("Your balance is %g€\n", u1.balance);
    //struct account a1;
    printf("enter the title of account: \n");
    scanf("%s", a1.title);
    a1.user_account = u1;
    char res;
    printf("Do you want to enter the description of your account: y/n\n");
    do{
        scanf("%c", &res);
        if(res == 'y'){
            printf("Enter the description please (140 characters max): \n");
            scanf("%s", a1.description);
            break;
        }else if(res == 'n'){
            printf("okay, your description will be empty, if you want to change your description later you can do it in the menu \n");
            break;
        }else{
            printf("invalid input, please enter 'y' or 'n'\n");
        }
    }while(res != 'y' || res != 'n');
    printf("Done ! Your account has been successfully created\n");
}

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
    while(1){
        connect_fd = accept(sock_fd, (struct sockaddr*)&client, &len);
        if(connect_fd < 0){
            syserr("error of connexion with client\n");
            return 1;
        }else{
            printf("connexion with client successful\n");
        
        }
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
        /* if(!strcmp(buffer,"exit")){
            printf("exit...\n");
            break;
        }else{
            strncpy(buffer, "Undefined command", 1024);
            send(connect_fd, buffer, strlen(buffer), 0);
            bzero(buffer, sizeof(buffer));
        } */
       /* printf("j'ai obtenu : %s\n", buffer);
        nb = read(STDIN_FILENO, &buffer, sizeof(buffer));
        buffer[strcspn(buffer, "\n")] = 0;
        s = send(sock_fd, buffer, nb, 0);
        r = recv(connect_fd, buffer, 1024, 0); */
       /* if((childpid = fork()) == 0){
			close(sock_fd);
        } */
        nbClient++;
        printf("nb client: %d\n", nbClient);
        log[0] = '1';
        send(connect_fd, log, strlen(log), 0);
        if(nbClient > 1){
            log[0] = '\0';
        }
       /* while (1){
            recv(connect_fd, buffer, 1024, 0);
            if(strcmp(buffer, "!exit") == 0) {
                printf("Disconnected");
                break;
            } else {
           // printf("Return error", buffer);
            //strncpy(buffer, "Undefined command", 1024);
                printf("Client: %s\n", buffer);
                scanf("%s", &buffer[0]);
                send(connect_fd, buffer, strlen(buffer), 0);
                bzero(buffer, sizeof(buffer));
            }
        } */
    }
    close(connect_fd);
    return 0;
}

