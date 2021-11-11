#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "struct.h"

#define PORT 8888 

/*Client : 
    Créer le socket
    Assigner les adresses
    Se connecter au server
    Communication
*/

// faire de l'UDP, (et sinon mettre un compteur de paquet pour verifier qu'il n'y a pas de paquet perdu ça sera un bonus) 

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


void syserr(char *message) {
    perror(message);
}

void display_menu_starting(){
    printf("- Welcome to Tricount App -\n");
    printf(" - Main menu - \n");
}

int main(int argc, char* argv[]){
    struct account *a1 = NULL;
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[1024], reponse[2048];
    if(sock_fd == -1){
        syserr("Error with creation of socket\n");
        return 1;
    }else{
        printf("Socket creation successful\n");
    }

    struct sockaddr_in server;

    server.sin_addr.s_addr = htonl(INADDR_ANY); 
    server.sin_port = htons(PORT); 
    server.sin_family = AF_INET;
    
    socklen_t len = sizeof(server);

    int connect_fd;
    connect_fd = connect(sock_fd, (struct sockaddr*) &server, sizeof(server));
    if(connect_fd < 0){
        syserr("Error of connexion with the server\n");
        return 1;
    }else{
        printf("Connexion successful !\n");
        display_menu_starting();
        if(a1 == NULL){
            a1 = malloc(sizeof(*a1));
            init_account(*a1);
        }
        while(1){

        }
    }

    
   /* int nb = read(STDIN_FILENO, &buffer, sizeof(buffer));
    buffer[strcspn(buffer, "\n")] = 0;
    int s = send(sock_fd, buffer, nb, MSG_CONFIRM);
   // int r = recvfrom(sock_fd, buffer, 1024, MSG_WAITALL, (struct sockaddr*)&server, &len); */
    // scanf("%[^\n]%*c", buffer); 
    buffer[strcspn(buffer, "\n")] = 0;
    send(sock_fd, buffer, 5, 0);
    recv(connect_fd, reponse, 1024, MSG_WAITALL);

   // while(1){
        // get all words, not only the first
        
        
       // int r = recv(connect_fd, buffer, 1024, MSG_WAITALL);
        // buffer[r] = '\0';
       // printf("%s\n", buffer);  // mon buffer se remplit bien       
        /*  printf("reponse du server : %s\n", buffer);
          if(r >0){
            break;
        }
        for(int i =0; i<s; i++){
            buffer[i] = '\0';
            printf("je recommence");
        } */
       
     //   recv(connect_fd, reponse, 1024, 0);
   // } 
    close(sock_fd);
    return 0;
}

