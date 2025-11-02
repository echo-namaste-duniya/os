#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper to print a boxed Gantt chart from segments
void print_gantt_segments(char **labels, int start[], int end[], int m) {
    if(m <= 0) return;
    int i, j;
    // Top border (underscores)
    for(i = 0; i < m; i++) {
        int seg = end[i] - start[i];
        int box = seg * 2 + 6; // minimum width depends on segment length
        for(j = 0; j < box; j++) putchar('_');
        if(i < m-1) putchar(' ');
    }
    putchar('\n');

    // Label line
    for(i = 0; i < m; i++) {
        int seg = end[i] - start[i];
        int box = seg * 2 + 6;
        int inner = box - 2;
        int labellen = (int)strlen(labels[i]);
        int pad = inner - labellen;
        int left = pad / 2;
        int right = pad - left;
        putchar('|');
        for(j = 0; j < left; j++) putchar(' ');
        fputs(labels[i], stdout);
        for(j = 0; j < right; j++) putchar(' ');
        putchar('|');
        if(i < m-1) putchar(' ');
    }
    putchar('\n');

    // Bottom border (dashes)
    for(i = 0; i < m; i++) {
        int seg = end[i] - start[i];
        int box = seg * 2 + 6;
        for(j = 0; j < box; j++) putchar('-');
        if(i < m-1) putchar(' ');
    }
    putchar('\n');

    // Time markers: print start times spaced appropriately
    for(i = 0; i < m; i++) {
        int seg = end[i] - start[i];
        int box = seg * 2 + 6;
        // print start time
        printf("%d", start[i]);
        int printed = 0;
        int tmp = start[i];
        if(tmp == 0) printed = 1; else { while(tmp) { printed++; tmp /= 10; } }
        int spaces = box - printed;
        for(j = 0; j < spaces; j++) putchar(' ');
    }
    // print final end time
    printf("%d\n", end[m-1]);
}

void fcfs(int n, int at[], int bt[]) {
    int *ct = malloc(n*sizeof(int)); // completion times
    int *wt = malloc(n*sizeof(int));
    int *tat = malloc(n*sizeof(int));
    int time = 0;

    // We'll collect segments for the Gantt chart
    char **labels = malloc(n * sizeof(char*));
    int *starts = malloc(n * sizeof(int));
    int *ends = malloc(n * sizeof(int));
    int seg_count = 0;

    for(int i = 0; i < n; i++) {
        if(time < at[i]) time = at[i];
        starts[seg_count] = time;
        time += bt[i];
        ends[seg_count] = time;
        labels[seg_count] = malloc(16);
        sprintf(labels[seg_count], "p%d", i+1);
        seg_count++;

        ct[i] = time;
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }

    double awt=0, atat=0;
    for(int i=0;i<n;i++) { awt += wt[i]; atat += tat[i]; }
    awt /= n; atat /= n;

    printf("Average waiting time for FCFS is: %.2f\n", awt);
    printf("Average turn-around time for FCFS is: %.2f\n", atat);

    // Print improved Gantt chart
    print_gantt_segments(labels, starts, ends, seg_count);

    for(int i=0;i<seg_count;i++) free(labels[i]);
    free(labels); free(starts); free(ends);
    free(ct); free(wt); free(tat);
}

void sjf_nonpreemptive(int n, int at[], int bt[]) {
    int *ct = malloc(n*sizeof(int));
    int *wt = malloc(n*sizeof(int));
    int *tat = malloc(n*sizeof(int));
    int *done = calloc(n,sizeof(int));
    int completed = 0, time = 0;

    // Prepare arrays for Gantt segments (max n segments)
    char **labels = malloc(n * sizeof(char*));
    int *starts = malloc(n * sizeof(int));
    int *ends = malloc(n * sizeof(int));
    int seg_count = 0;

    while(completed < n) {
        int idx = -1; int minb = 1e9;
        for(int i=0;i<n;i++) {
            if(!done[i] && at[i] <= time) {
                if(bt[i] < minb) { minb = bt[i]; idx = i; }
                else if(bt[i] == minb && at[i] < at[idx]) idx = i;
            }
        }
        if(idx == -1) { time++; continue; }
        starts[seg_count] = time;
        time += bt[idx];
        ends[seg_count] = time;
        labels[seg_count] = malloc(16);
        sprintf(labels[seg_count], "p%d", idx+1);
        seg_count++;

        ct[idx] = time;
        tat[idx] = ct[idx] - at[idx];
        wt[idx] = tat[idx] - bt[idx];
        done[idx] = 1;
        completed++;
    }

    double awt=0, atat=0;
    for(int i=0;i<n;i++) { awt += wt[i]; atat += tat[i]; }
    awt /= n; atat /= n;

    printf("Average waiting time for SJF (non-preemptive) is: %.2f\n", awt);
    printf("Average turn-around time for SJF (non-preemptive) is: %.2f\n", atat);

    // Print improved Gantt
    print_gantt_segments(labels, starts, ends, seg_count);

    for(int i=0;i<seg_count;i++) free(labels[i]);
    free(labels); free(starts); free(ends);
    free(ct); free(wt); free(tat); free(done);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    if(scanf("%d", &n)!=1) return 0;
    int *at = malloc(n*sizeof(int));
    int *bt = malloc(n*sizeof(int));
    printf("Process Arrival_time Burst_time\n");
    for(int i=0;i<n;i++) {
        printf("p%d: ", i+1);
        scanf("%d %d", &at[i], &bt[i]);
    }

    while(1) {
        printf("\nEnter the choice: 1. FCFS 2. SJF (non-preemptive) 3. Exit\n");
        int choice; scanf("%d", &choice);
        if(choice==1) fcfs(n, at, bt);
        else if(choice==2) sjf_nonpreemptive(n, at, bt);
        else break;
    }

    free(at); free(bt);
    return 0;
}
