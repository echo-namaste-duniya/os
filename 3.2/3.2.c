#include <stdio.h>
#include <limits.h>
#include <string.h>

struct Process {
    char name[5];
    int at, bt, remaining_bt, ct, wt, tat, completed;
};

// Sort by Arrival Time
void sortByArrival(struct Process p[], int n) {
    struct Process temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (p[i].at > p[j].at) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

// FCFS Scheduling
void fcfs(struct Process p[], int n) {
    int time = 0;
    float total_wt = 0, total_tat = 0;

    sortByArrival(p, n);

    printf("\nGANTT CHART (FCFS):\n");
    printf("________________________________________\n|");

    int timeline[50];
    timeline[0] = 0;

    for (int i = 0; i < n; i++) {
        if (time < p[i].at)
            time = p[i].at;

        printf(" %s |", p[i].name);
        time += p[i].bt;
        timeline[i + 1] = time;

        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        total_wt += p[i].wt;
        total_tat += p[i].tat;
    }

    printf("\n________________________________________\n");

    for (int i = 0; i <= n; i++)
        printf("%d\t", timeline[i]);

    printf("\n\nAverage Waiting Time for FCFS: %.2f", total_wt / n);
    printf("\nAverage Turnaround Time for FCFS: %.2f\n", total_tat / n);
}

// SJF (Preemptive) Scheduling with detailed Gantt chart
void sjf_preemptive(struct Process p[], int n) {
    int completed = 0, time = 0, min_bt, idx = -1, prev = -1;
    float total_wt = 0, total_tat = 0;
    int gantt_time[100], gantt_index = 0;
    char gantt_proc[100][5];

    sortByArrival(p, n);
    for (int i = 0; i < n; i++) {
        p[i].remaining_bt = p[i].bt;
        p[i].completed = 0;
    }

    printf("\nGANTT CHART (SJF Preemptive):\n");
    printf("________________________________________\n|");

    while (completed != n) {
        min_bt = INT_MAX;
        idx = -1;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].completed == 0 && p[i].remaining_bt < min_bt && p[i].remaining_bt > 0) {
                min_bt = p[i].remaining_bt;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        // Record process change in Gantt chart
        if (prev != idx) {
            printf(" %s |", p[idx].name);
            gantt_time[gantt_index] = time;
            strcpy(gantt_proc[gantt_index], p[idx].name);
            gantt_index++;
            prev = idx;
        }

        p[idx].remaining_bt--;
        time++;

        if (p[idx].remaining_bt == 0) {
            p[idx].completed = 1;
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            total_wt += p[idx].wt;
            total_tat += p[idx].tat;
            completed++;
        }
    }

    gantt_time[gantt_index] = time;

    printf("\n________________________________________\n");

    // Print time points under Gantt chart
    for (int i = 0; i <= gantt_index; i++) {
        printf("%d\t", gantt_time[i]);
    }

    printf("\n\nAverage Waiting Time for SJF (Preemptive): %.2f", total_wt / n);
    printf("\nAverage Turnaround Time for SJF (Preemptive): %.2f\n", total_tat / n);
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
    }

    while (1) {
        printf("\n\nEnter your choice:\n1. FCFS\n2. SJF (Preemptive)\n3. Exit\nChoice: ");
        scanf("%d", &choice);

        if (choice == 3)
            break;
        else if (choice == 1)
            fcfs(p, n);
        else if (choice == 2)
            sjf_preemptive(p, n);
        else
            printf("Invalid choice!\n");
    }

    return 0;
}
