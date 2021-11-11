#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

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
    char buffer[1024];
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
    
    int connect_fd;
            connect_fd = connect(sock_fd, (struct sockaddr*) &server, sizeof(server));
        if(connect_fd < 0){
            syserr("Error of connexion with the server\n");
            return 1;
        }
        printf("Connexion successful !\n");
    while(1){
        // get all words, not only the first
        scanf("%[^\n]%*c", buffer);  
        printf("%s\n", buffer);  // mon buffer se remplit bien      
        send(connect_fd, buffer, strlen(buffer), MSG_CONFIRM);
    }

    return 0;
}

