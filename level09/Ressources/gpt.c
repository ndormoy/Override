#include <stdio.h>
#include <string.h>

struct Message {
    char username[140];
    char msg[1024];
    int flag;
    int count;
    int length;
};

void handle_msg(struct Message *msg) {
    memset(msg, 0, sizeof(struct Message));
    
    set_username(msg);
    set_msg(msg);
    
    printf("Message: %s\n", msg->msg);
}

void set_username(struct Message *msg) {
    memset(msg->username, 0, sizeof(msg->username));
    
    printf("Enter username: ");
    fgets(msg->username, sizeof(msg->username), stdin);
    
    printf("Username: %s", msg->username);
}

void set_msg(struct Message *msg) {
    memset(msg->msg, 0, sizeof(msg->msg));
    
    printf("Enter message: ");
    fgets(msg->msg, sizeof(msg->msg), stdin);
    
    printf("Message: %s", msg->msg);
}

int main() {
    printf("Starting program...\n");
    
    struct Message msg;
    handle_msg(&msg);
    
    return 0;
}
