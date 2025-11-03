#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Process {
    char name[5];
    int at, bt, ct, tat, wt, rt;
    int remaining_bt;
};

// Function to calculate FCFS
void fcfs(struct Process p[], int n) {
    int time = 0;
    float total_wt = 0, total_tat = 0;

    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].at > p[j].at) {
                struct Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    printf("\nGANTT CHART:\n");
    printf("-------------------------------------------------\n");
    printf("|");

    for (int i = 0; i < n; i++) {
        if (time < p[i].at)
            time = p[i].at;
        time += p[i].bt;
        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        total_tat += p[i].tat;
        total_wt += p[i].wt;
        printf(" %s |", p[i].name);
    }

    printf("\n-------------------------------------------------\n");

    time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].at)
            time = p[i].at;
        printf("%d\t", time);
        time += p[i].bt;
    }
    printf("%d\n", time);

    printf("\nAverage Waiting Time for FCFS: %.2f", total_wt / n);
    printf("\nAverage Turnaround Time for FCFS: %.2f\n", total_tat / n);
}

// Function to calculate Round Robin
void round_robin(struct Process p[], int n, int tq) {
    int time = 0, done;
    float total_wt = 0, total_tat = 0;
    int completed = 0;
    char gantt[200] = "";

    for (int i = 0; i < n; i++)
        p[i].remaining_bt = p[i].bt;

    printf("\nGANTT CHART:\n");
    printf("-------------------------------------------------\n");
    printf("|");

    while (completed < n) {
        done = 1;
        for (int i = 0; i < n; i++) {
            if (p[i].remaining_bt > 0 && p[i].at <= time) {
                done = 0;
                if (p[i].remaining_bt > tq) {
                    printf(" %s |", p[i].name);
                    time += tq;
                    p[i].remaining_bt -= tq;
                } else {
                    printf(" %s |", p[i].name);
                    time += p[i].remaining_bt;
                    p[i].ct = time;
                    p[i].tat = p[i].ct - p[i].at;
                    p[i].wt = p[i].tat - p[i].bt;
                    total_tat += p[i].tat;
                    total_wt += p[i].wt;
                    p[i].remaining_bt = 0;
                    completed++;
                }
            }
        }
        if (done)
            time++;
    }

    printf("\n-------------------------------------------------\n");
    printf("\nAverage Waiting Time for Round Robin: %.2f", total_wt / n);
    printf("\nAverage Turnaround Time for Round Robin: %.2f\n", total_tat / n);
}

int main() {
    int n, choice;
    int tq = 2;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process *p = (struct Process *)malloc(n * sizeof(struct Process));

    printf("\nProcess\tArrival Time\tBurst Time\n");
    for (int i = 0; i < n; i++) {
        sprintf(p[i].name, "p%d", i + 1);
        scanf("%d %d", &p[i].at, &p[i].bt);
    }

    while (1) {
        printf("\nEnter your choice:\n1. FCFS\n2. Round Robin (preemptive)\n3. Exit\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fcfs(p, n);
                break;
            case 2:
                round_robin(p, n, tq);
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice\n");
        }
    }
    free(p);
    return 0;
}
