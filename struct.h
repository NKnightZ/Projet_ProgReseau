#ifndef __TRICOUNT_STRUCT__
#define __TRICOUNT_STRUCT__

struct user{
    char name[20];
    double balance;
};

struct spent{
    char name_spent[50];
    struct user user_spent;
    char date[20];
    double amount;
};

struct account{ // <- à mettre au niveau du serveur, le client n'a pas besoin de crée un compte à chaque fois qu'il se connecte, sa sera le serveur qui lui proposera de réjoindre son compte
    char title[50];
    struct user user_account;
    char description[140]; // for example hotel, market, travel
    double total;
};

#endif