#include "server.h"

struct user u1;
struct user u2;
struct user u3;
struct user u4;

int nb_user_account;

struct account a1;

struct client client_array[MAX_CLIENTS];
bool clients[MAX_CLIENTS];

int sock_fd;
struct sockaddr_in server, client;

socklen_t len;
char buffer[BUFFER_SIZE];
char response[RESPONSE_SIZE];
int val_return;

void syserr(char *message){
    perror(message);
}

bool accept_client(){
    struct sockaddr_in new_client = {0};
    socklen_t length = sizeof(new_client);
    val_return = accept(sock_fd, (struct sockaddr *)&new_client, &length);
    if(val_return < 0){
        syserr("error of accept");
        return true;
    }
    printf("accept of client successful\n");
    for(int i = 0; i < MAX_CLIENTS; i++){
        client_array[i].fd = val_return;
    }
    return false;
}

bool reading_client(){
    client_array[0].in = fdopen(client_array[0].fd, "r");
    if(client_array[0].in == NULL){
        syserr("error of fdopen");
        return true;
    }
    size_t nb_read = fread(buffer, sizeof(char), 1024, client_array[0].in);
    if(nb_read < 1){
        syserr("error of fread");
        return true;
    }
    return false;
}

bool send_client(FILE *f){
    size_t nb_write = fwrite(response, sizeof(char), sizeof(response), f); // sending the response to the client
    if(nb_write < 0){
        syserr("error of fwrite");
        return true;
    }
    if(fflush(f)){
        syserr("error of fflush");
        return true;
    }
    for (size_t i = 0; i < RESPONSE_SIZE; i++){
        response[i] = '\0';
    }
    return false;
}

void send_spend(struct user *user_source, int32_t amount){
    for(int i = 0; i < MAX_LIST_SIZE; i++){
        if(strcmp(a1.list_user[i].name, user_source->name)){
            a1.list_user[i].balance -= (unsigned int)amount / (unsigned int)nb_user_account;
        }else{
            a1.list_user[i].balance += (unsigned int)amount - ((unsigned int)amount / (unsigned int)nb_user_account);
        }
    }
    a1.total += amount;
}

void send_refund(struct user *user_from, struct user *user_source, int32_t amount){
    struct user user_temp;
    for(int i = 0; i < MAX_LIST_SIZE; i++){
        if(strcmp(a1.list_user[i].name, user_source->name) == 0){     
            user_temp = a1.list_user[i]; 
            for(int i = 0; i < MAX_LIST_SIZE; i++){
                if(strcmp(a1.list_user[i].name, user_from->name) == 0){
                    user_temp.balance += (unsigned int)amount;
                    a1.list_user[i].balance -= (unsigned int)amount;
                }     
            } 
            a1.list_user[i] = user_temp; 
        } 
    }
}

int main(int argc, char *agrv[]){
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    int max = sock_fd;

    memset(clients, 1, sizeof(clients));
    char parsed[BUFFER_SIZE][BUFFER_SIZE];

    /* USER */
    strcpy(u1.name, "foozy");
    u1.balance = 0;

    strcpy(u2.name, "barry");
    u2.balance = 0;

    strcpy(u3.name, "Damien");
    u3.balance = 0;
    
    strcpy(u4.name, "Mike");
    u4.balance = 0;

    /* ACCOUNT */
    strcpy(a1.title, "travel");
    a1.list_user[0] = u1;
    a1.list_user[1] = u2;
    a1.list_user[2] = u3;
    a1.list_user[3] = u4;
    a1.total = 0;

    for(int i = 0; i < MAX_LIST_SIZE; i++){
        if(strcmp(a1.list_user[i].name, "\0")){
            nb_user_account++;
        }
    }

    if(sock_fd == -1){
        syserr("Error with creation of socket\n");
        return 1;
    }else{
        printf("Socket creation successful\n");
    }

    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);
    server.sin_family = AF_INET;

    if(bind(sock_fd, (struct sockaddr *)&server, sizeof(server)) == -1){
        syserr("Error with binding of socket\n");
        return 1;
    }else{
        printf("Bind of socket successful\n");
    }

    len = sizeof(client);

    if(listen(sock_fd, 10) == 0){
        printf("[+]Listening....\n");
    }else{
        syserr("[-]Error in binding.\n");
        return 1;
    }

    while(true){
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sock_fd, &set);

        for(int i = 0; i < MAX_CLIENTS; i++){
            if(clients[i]){
                FD_SET(client_array[i].fd, &set);
            }
        }

        int nbSelect = select(max + 1, &set, NULL, NULL, NULL);

        if(nbSelect < 0){
            syserr("error of select");
            return 1;
        }

        if(FD_ISSET(sock_fd, &set)){
            accept_client();
        }else{
            syserr("error of accept client");
            return 1;
        }

        if(reading_client() == false){
            if(strcmp(buffer, "state") == 0){
                client_array[0].out = fdopen(client_array[0].fd, "w");
                if(client_array[0].out == NULL){
                    syserr("error of fdopen");
                    return 1;
                }
                printf("sending the state of account...\n");
                memcpy(response, &a1, sizeof(a1));
                if(send_client(client_array[0].out)){
                    printf("error of sending..\n");
                    return 1;
                }
            }else{
                /* PARSING */
                int j = 0;
                int k = 0;
                char temp[BUFFER_SIZE];
                for(size_t i = 0; i < strlen(buffer); i++){
                    if(isspace(buffer[i])){
                        temp[i] = '\0';
                        strcpy(parsed[j], temp);
                        for (size_t k = 0; k < BUFFER_SIZE; k++){
                            temp[k] = '\0';
                        }
                        k = -1;
                        j++;
                    }
                    temp[k] = buffer[i];
                    if(buffer[i+1] == '\0'){
                        buffer[i+1] = '\0';
                        strcpy(parsed[j], temp);
                    }
                    k++;
                }
                for(int i = 0; i < MAX_LIST_SIZE; i++){
                    if(strcmp(parsed[0], a1.list_user[i].name) == 0){
                        struct user user_source = a1.list_user[i];
                        int amount;
                        if(strcmp(parsed[1], "spend") == 0){   
                            amount = atoi(parsed[2]);
                            if(amount > 0){
                                printf("A user do a spend\n");
                                send_spend(&user_source, amount);
                                memcpy(response, "Operation sucessful !", strlen("Operation sucessful !"));
                                printf("%s\n", response);
                            }else{
                                memcpy(response, "Operation failed.", strlen("Operation failed."));
                                printf("%s\n", response);
                            }   
                        }else if(strcmp(parsed[1], "refund") == 0){
                            amount = atoi(parsed[3]);
                            for(int i = 0; i < MAX_LIST_SIZE; i++){
                                if(strcmp(parsed[2], a1.list_user[i].name) == 0){
                                    struct user user_from = a1.list_user[i];
                                    if(amount > 0){
                                        printf("A user do a refund\n");
                                        send_refund(&user_from, &user_source, amount);
                                        memcpy(response, "Operation sucessful !", strlen("Operation sucessful !"));
                                        printf("%s\n", response);
                                    }else{
                                        memcpy(response, "Operation failed.", strlen("Operation failed."));
                                        printf("%s\n", response);
                                    }
                                    break;
                                }else{
                                    memcpy(response, "user not found", strlen("user not found"));
                                }
                            } 
                        }
                        break;
                    }else{
                        memcpy(response, "user not found", strlen("user not found"));
                    } 
                }
                client_array[0].out = fdopen(client_array[0].fd, "w");
                if(client_array[0].out == NULL){
                    syserr("error of fdopen");
                    return 1;
                }
                if(send_client(client_array[0].out)){
                    printf("error of sending..\n");
                    return 1;
                }
            }
        }else{
            printf("unknown command\n");
        }
    }
    if(fclose(client_array[0].in) != 0){
        syserr("Error of fclose in");
        return 1;
    }
    if(fclose(client_array[0].out) != 0){
        syserr("Error of fclose out");
        return 1;
    }
    if(close(sock_fd)){
        syserr("error of close socket");
        return 1;
    }
    return 0;
}
