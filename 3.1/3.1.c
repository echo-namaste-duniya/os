#include <stdio.h>

struct Process {
    char name[5];
    int at, bt, ct, wt, tat, completed;
};

void findAverageTime(struct Process p[], int n, int choice) {
    int time = 0, completed = 0;
    float total_wt = 0, total_tat = 0;

    printf("\nGANTT CHART:\n");
    printf("__________________________\n|");

    if (choice == 1) {  // FCFS
        for (int i = 0; i < n; i++) {
            if (time < p[i].at)
                time = p[i].at;
            p[i].ct = time + p[i].bt;
            time = p[i].ct;

            p[i].tat = p[i].ct - p[i].at;
            p[i].wt = p[i].tat - p[i].bt;
            total_wt += p[i].wt;
            total_tat += p[i].tat;

            printf(" %s |", p[i].name);
        }
    } 
    else if (choice == 2) {  // SJF (non-preemptive)
        while (completed != n) {
            int idx = -1, min_bt = 9999;
            for (int i = 0; i < n; i++) {
                if (p[i].at <= time && p[i].completed == 0) {
                    if (p[i].bt < min_bt) {
                        min_bt = p[i].bt;
                        idx = i;
                    }
                }
            }
            if (idx == -1)
                time++;
            else {
                p[idx].ct = time + p[idx].bt;
                time = p[idx].ct;
                p[idx].tat = p[idx].ct - p[idx].at;
                p[idx].wt = p[idx].tat - p[idx].bt;
                total_wt += p[idx].wt;
                total_tat += p[idx].tat;
                p[idx].completed = 1;
                completed++;
                printf(" %s |", p[idx].name);
            }
        }
    }

    printf("\n__________________________\n");
    time = 0;
    printf("0");
    for (int i = 0; i < n; i++) {
        time += p[i].bt;
        printf("    %d", time);
    }

    printf("\n\nAverage Waiting Time: %.2f", total_wt / n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
}

int main() {
    struct Process p[10];
    int n, choice;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("\nEnter Process Name, Arrival Time, Burst Time:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d name: ", i + 1);
        scanf("%s", p[i].name);
        printf("Arrival time: ");
        scanf("%d", &p[i].at);
        printf("Burst time: ");
        scanf("%d", &p[i].bt);
        p[i].completed = 0;
    }

    while (1) {
        printf("\n\nEnter your choice:\n1. FCFS\n2. SJF (Non-preemptive)\n3. Exit\nChoice: ");
        scanf("%d", &choice);

        if (choice == 3)
            break;
        else if (choice == 1 || choice == 2)
            findAverageTime(p, n, choice);
        else
            printf("Invalid choice!\n");
    }

    return 0;
}
