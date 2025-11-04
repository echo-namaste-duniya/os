#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>

#define MAX_SIZE 128
#define QUEUE_KEY 0x1234   // same key as sender

struct msgbuf {
    long mtype;
    char mtext[MAX_SIZE];
};

int main() {
    int msgid = msgget(QUEUE_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    struct msgbuf msg;

    printf("Receiver ready. Waiting for messages...\n");

    while (1) {
        if (msgrcv(msgid, &msg, sizeof(msg.mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        if (strcmp(msg.mtext, "exit") == 0) {
            printf("Receiver exiting...\n");
            break;
        }

        for (int i = 0; msg.mtext[i]; i++) {
            msg.mtext[i] = toupper(msg.mtext[i]);
        }

        printf("\nReceived message (type %ld): %s\n", msg.mtype, msg.mtext);
    }

    printf("\nCleaning up message queue...\n");
    if (msgctl(msgid, IPC_RMID, NULL) == 0)
        printf("Message queue successfully removed.\n");
    else
        perror("Failed to remove message queue");

    return 0;
}
