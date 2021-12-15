/* Client peut envoie de 3 paquet : 
-   rembourser
-   depenser
-   voir l'etat des comptes

Serveur peut envoyer 1 paquet : 
-   savoir l'etat des comptes par le serveur et l'envoyer au client

// pas de creation de compte, pas rejoindre un compte de server */

#define MAX_CLIENT_NAME_LENGTH 120
#define MAX_LIST_SIZE 10
#define MAX_TITLE_LENGTH 10

struct user{
    char name[MAX_CLIENT_NAME_LENGTH];
    int32_t balance; // int32_t car balance peut etre negative
};

struct account{ // <- à mettre au niveau du serveur, le client n'a pas besoin de crée un compte à chaque fois qu'il se connecte, sa sera le serveur qui lui proposera de réjoindre son compte
    char title[MAX_TITLE_LENGTH];
    struct user list_user[MAX_LIST_SIZE];
    // char description[140]; // for example hotel, market, travel
    int32_t total;
};