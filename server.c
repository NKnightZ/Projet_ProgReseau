#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

    while(1){
        connect_fd = accept(sock_fd, (struct sockaddr*)&client, &len);
        if(connect_fd < 0) {
            syserr("error of connexion with client\n");
        }
        printf("connexion with client successful\n");
    }
    return 0;
}

