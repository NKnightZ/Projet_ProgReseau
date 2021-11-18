#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define PORT 8080 

// faire de l'UDP, (et sinon mettre un compteur de paquet pour verifier qu'il n'y a pas de paquet perdu ça sera un bonus) 
// ne pas oublier fflush apres un fwrite pour forcer l'envoie

/* void init_account(struct account *a1, struct user *u1){
    printf("To start you must to create a account\n");
    
    //struct account a1;
    printf("enter the title of account: \n");
    scanf("%s", a1->title);
    a1->user_account = *u1;
    char res;
    printf("Do you want to enter the description of your account: y/n\n");
    do{
        scanf("%c", &res);
        if(res == 'y'){
            printf("Enter the description please (140 characters max): \n");
            scanf("%s", a1->description);
            break;
        }else if(res == 'n'){
            printf("okay, your description will be empty, if you want to change your description later you can do it in the menu \n");
            break;
        }else{
            printf("invalid input, please enter 'y' or 'n'\n");
        }
    }while(res != 'y' || res != 'n');
    printf("Done ! Your account has been successfully created\n");
} */

void syserr(char *message) {
    perror(message);
}

// send_rembourse(from, source, montant)

// send_depense()



int main(int argc, char* argv[]){
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    FILE* fd = fdopen(sock_fd, "w");
   // char buffer[1024]; //, reponse[2048];
   
    if(sock_fd == -1){
        syserr("Error with creation of socket\n");
        return 1;
    }else{
        printf("Socket creation successful\n");
    }

    struct sockaddr_in server;
    // server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_addr.s_addr = htonl(INADDR_ANY); 
    server.sin_port = htons(PORT); 
    server.sin_family = AF_INET;
    
    socklen_t len = sizeof(server);

    int connect_fd;
    connect_fd = connect(sock_fd, (struct sockaddr*) &server, len);
    if(connect_fd < 0){
        syserr("Error of connexion with the server\n");
        return 1;
    }else{
        printf("Connexion successful !\n");
    }

    // fwrite(&u1, sizeof(u1), 1, fd);  
    fflush(fd);
    
    // printf("%s\n", argv[1]);
   // size_t w = fwrite( 
    close(sock_fd);
    return 0;
}