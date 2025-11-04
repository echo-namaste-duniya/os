#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_SIZE 128
#define QUEUE_KEY 0x1234   // any constant

struct msgbuf {
    long mtype;
    char mtext[MAX_SIZE];
};

int main() {
    int msgid = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    struct msgbuf msg;

    while (1) {
        printf("\nEnter message type (>0) or 0 to exit: ");
        scanf("%ld", &msg.mtype);
        getchar(); // consume newline

        if (msg.mtype == 0) {
            printf("Sender exiting...\n");
            strcpy(msg.mtext, "exit");
            msg.mtype=1;
            if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
            perror("msgsnd");
            }
            break;
        }

        printf("Enter message: ");
        fgets(msg.mtext, MAX_SIZE, stdin);
        msg.mtext[strcspn(msg.mtext, "\n")] = '\0';

        if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
            perror("msgsnd");
        }
    }

    return 0;
}
