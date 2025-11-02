#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// Helper to print a boxed Gantt chart from segments
void print_gantt_segments(char **labels, int start[], int end[], int m) {
    if(m <= 0) return;
    int i, j;
    for(i = 0; i < m; i++) {
        int seg = end[i] - start[i];
        int box = seg * 2 + 6;
        for(j = 0; j < box; j++) putchar('_');
        if(i < m-1) putchar(' ');
    }
    putchar('\n');
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
    for(i = 0; i < m; i++) {
        int seg = end[i] - start[i];
        int box = seg * 2 + 6;
        for(j = 0; j < box; j++) putchar('-');
        if(i < m-1) putchar(' ');
    }
    putchar('\n');
    for(i = 0; i < m; i++) {
        int seg = end[i] - start[i];
        int box = seg * 2 + 6;
        printf("%d", start[i]);
        int printed = 0; int tmp = start[i];
        if(tmp == 0) printed = 1; else { while(tmp) { printed++; tmp /= 10; } }
        int spaces = box - printed;
        for(j = 0; j < spaces; j++) putchar(' ');
    }
    printf("%d\n", end[m-1]);
}

void fcfs(int n,int at[],int bt[]){
    int *ct=malloc(n*sizeof(int)); int *wt=malloc(n*sizeof(int)); int *tat=malloc(n*sizeof(int));
    int t=0;
    // collect segments
    char **labels = malloc(n * sizeof(char*)); int *starts = malloc(n * sizeof(int)); int *ends = malloc(n * sizeof(int)); int seg=0;
    for(int i=0;i<n;i++){
        if(t<at[i]) t=at[i];
        starts[seg]=t; t+=bt[i]; ends[seg]=t; labels[seg]=malloc(16); sprintf(labels[seg], "p%d", i+1); seg++;
        ct[i]=t; tat[i]=ct[i]-at[i]; wt[i]=tat[i]-bt[i];
    }
    double awt=0,atat=0; for(int i=0;i<n;i++){awt+=wt[i]; atat+=tat[i];}
    printf("Average waiting time for FCFS is: %.2f\n",awt/n);
    printf("Average turn-around time for FCFS is: %.2f\n",atat/n);
    // print improved Gantt
    extern void print_gantt_segments(char **labels, int start[], int end[], int m);
    print_gantt_segments(labels, starts, ends, seg);
    for(int i=0;i<seg;i++) free(labels[i]); free(labels); free(starts); free(ends);
    free(ct); free(wt); free(tat);
}

void sjf_preemptive(int n,int at[],int bt[]){
    int *rt=malloc(n*sizeof(int)); for(int i=0;i<n;i++) rt[i]=bt[i];
    int complete=0,t=0,minm; int shortest=-1; int finish_time; int check=0;
    int *ct=malloc(n*sizeof(int));
    // prepare segments dynamic arrays
    char **labels = malloc(1000 * sizeof(char*)); int *starts = malloc(1000 * sizeof(int)); int *ends = malloc(1000 * sizeof(int)); int segcount=0;
    int last = -1;

    while(complete!=n){
        minm = INT_MAX; shortest = -1; check = 0;
        for(int j=0;j<n;j++){
            if(at[j]<=t && rt[j]>0 && rt[j]<minm){ minm=rt[j]; shortest=j; check=1; }
        }
        if(check==0){ t++; continue; }
        // if switching to a new process, start a new segment
        if(last != shortest){
            labels[segcount] = malloc(16);
            sprintf(labels[segcount], "p%d", shortest+1);
            starts[segcount] = t;
            // previous segment end will be filled when switch occurs
            segcount++;
            last = shortest;
        }
        // execute one time unit
        rt[shortest]--;
        t++;
        // if current segment ends (process finished or will switch next), update end
        if(rt[shortest] == 0){
            complete++; finish_time = t; ct[shortest]=finish_time;
            // set end for current segment
            ends[segcount-1] = t;
            last = -1; // next loop will detect new process
        } else {
            // peek next shortest for next time to detect switch
            int nextmin = INT_MAX; int nextshort = -1;
            for(int j=0;j<n;j++){
                if(at[j]<=t && rt[j]>0 && rt[j]<nextmin){ nextmin=rt[j]; nextshort=j; }
            }
            if(nextshort != shortest){
                ends[segcount-1] = t;
                last = -1;
            }
        }
    }
    int *wt=malloc(n*sizeof(int)); int *tat=malloc(n*sizeof(int));
    double awt=0,atat=0;
    for(int i=0;i<n;i++){ tat[i]=ct[i]-at[i]; wt[i]=tat[i]-bt[i]; awt+=wt[i]; atat+=tat[i]; }
    printf("Average waiting time for SJF (preemptive) is: %.2f\n",awt/n);
    printf("Average turn-around time for SJF (preemptive) is: %.2f\n",atat/n);

    extern void print_gantt_segments(char **labels, int start[], int end[], int m);
    print_gantt_segments(labels, starts, ends, segcount);

    for(int i=0;i<segcount;i++) free(labels[i]); free(labels); free(starts); free(ends);
    free(rt); free(ct); free(wt); free(tat);
}

int main(){
    int n; printf("Enter the number of processes: "); if(scanf("%d",&n)!=1) return 0;
    int *at=malloc(n*sizeof(int)); int *bt=malloc(n*sizeof(int));
    printf("Process Arrival_time Burst_time\n");
    for(int i=0;i<n;i++){ printf("p%d: ",i+1); scanf("%d %d", &at[i], &bt[i]); }
    while(1){ printf("\nEnter the choice: 1. FCFS 2. SJF (preemptive) 3. Exit\n"); int c; scanf("%d",&c);
        if(c==1) fcfs(n,at,bt); else if(c==2) sjf_preemptive(n,at,bt); else break;
    }
    free(at); free(bt); return 0;
}
