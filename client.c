#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define PORT 8080
#define BUFF_SIZE 1024

void syserr(char *message) {
    perror(message);
}

// send_rembourse(from, source, montant)

// send_depense()

int main(int argc, char *argv[]){
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[BUFF_SIZE];

    if(sock_fd == -1){
        syserr("Error with creation of socket\n");
        return 1;
    }else{
        printf("Socket creation successful\n");
    }

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT); 
    server.sin_family = AF_INET;
    
    socklen_t len = sizeof(server);

    int connect_return;
    connect_return = connect(sock_fd, (struct sockaddr*) &server, len);
    
    if(connect_return < 0){
        syserr("Error of connection with the server\n");
        return 1;
    }else{
        printf("Connection successful !\n");
    }
    
    FILE* fdc = fdopen(sock_fd, "w+");

    if(fdc == NULL){
        syserr("error of fdopen");
        return 1;
    }
    
    if(argc > 1){
        printf("%s\n", argv[1]);
        size_t nb_write = fwrite(argv[1], sizeof(*argv), strlen(argv[1]), fdc);
    
        printf("number i write: %ld\n", nb_write);
    
        if(fflush(fdc)){
            syserr("fflush");
        }
    }
    size_t nbread = fread(&buffer, strlen(buffer), sizeof(BUFF_SIZE), fdc); // ce que je reçois du server
    printf("number i read: %ld\n", nbread);
    printf("%s\n", buffer);

    close(sock_fd);
    return 0;
}