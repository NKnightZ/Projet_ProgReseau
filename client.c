#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080 

/*Client : 
    Créer le socket
    Assigner les adresses
    Se connecter au server
    Communication
*/

// faire de l'UDP, (et sinon mettre un compteur de paquet pour verifier qu'il n'y a pas de paquet perdu ça sera un bonus) 

struct var{
    double spent;
    char* name;
    char* date;
    char* utility; // for example hotel, market, travel
};

void syserr(char *message) {
    perror(message);
}

int main(int argc, char* argv[]){
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

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

    if(connect(sock_fd, (struct sockaddr*) &server, sizeof(sock_fd)) == -1){
        syserr("Error of connexion with the server\n");
        return 1;
    }else{
        printf("Connexion successful !\n");
    }

    return 0;
}

