#define MAX_CLIENT_NAME_LENGTH 120
#define MAX_LIST_SIZE 4
#define MAX_TITLE_LENGTH 10

#define BUFFER_SIZE 1024
#define RESPONSE_SIZE 2048
#define MAX_CLIENTS 10

struct user{
    char name[MAX_CLIENT_NAME_LENGTH];
    uint32_t balance;
};

struct client{
    int fd;
    FILE *in;
    FILE *out;
};

struct account {
    char title[MAX_TITLE_LENGTH];
    struct user list_user[MAX_LIST_SIZE];
    int32_t total;
}__attribute__((__packed__));