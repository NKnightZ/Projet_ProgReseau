#include "tricount.h"

struct account a1;

char buffer[BUFFER_SIZE];
char response[RESPONSE_SIZE];

void syserr(char *message) {
    perror(message);
}

void display_state_info(){
    printf("\n");
    printf("Account title: %s\n", a1.title);
    int nb_user = 0;
    for(int i = 0; i < MAX_LIST_SIZE; i++){
        if(strcmp(a1.list_user[i].name, "\0")){
            nb_user++;
        }
    }
    for(int i = 0; i < nb_user; i++){
        printf("\n");
        printf("--- user %d ---\n", i+1);
        printf("name: %s\n", a1.list_user[i].name);
        printf("balance: %d\n", a1.list_user[i].balance);
    }
    printf("\n");
    printf("Total: %d\n", a1.total);
}

bool send_and_recieve(FILE *file){
    size_t nb_write = fwrite(buffer, sizeof(char), sizeof(buffer), file);
    if(nb_write < 0){
        syserr("error of fwrite");
        return true;
    }
    if(fflush(file)){
        syserr("error of fflush");
        return true;
    }
    size_t nb_read = fread(response, sizeof(char), sizeof(response), file);
    if(nb_read < 0){
        syserr("error of fread");
        return true;
    }
    return false;
}

int main(int argc, char *argv[]){
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
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
    
    FILE *fd = fdopen(sock_fd, "w+");

    if(fd == NULL){
        syserr("error of fdopen");
        return 1;
    }

    if(argc > 1 && argc <= 5){
        if(argc == 2 && strcmp(argv[1], "state") == 0){
            strcpy(buffer, argv[1]);
            if(send_and_recieve(fd)){
                return 1;
            }
            memcpy(&a1, response, sizeof(a1));
            display_state_info();
        }else if(argc == 4 && strcmp(argv[2], "spend") == 0){
            strcpy(buffer, argv[1]);
            strcat(buffer, " ");
            strcat(buffer, argv[2]);
            strcat(buffer, " ");
            strcat(buffer, argv[3]);
            if(send_and_recieve(fd)){
                return 1;
            }
            printf("%s\n", response);
        }else if(argc == 5 && strcmp(argv[2], "refund") == 0){
            strcpy(buffer, argv[1]);
            strcat(buffer, " ");
            strcat(buffer, argv[2]);
            strcat(buffer, " ");
            strcat(buffer, argv[3]);
            strcat(buffer, " ");
            strcat(buffer, argv[4]);
            if(send_and_recieve(fd)){
                return 1;
            }
            printf("%s\n", response);
        }else{
            printf("unknown command\n");
        }
    }else{
        printf("error with the number of argument, nothing will be send\n");
    }
    if(fclose(fd) != 0){
        syserr("Error of fclose fd");
        return 1;
    }
    return 0;
}