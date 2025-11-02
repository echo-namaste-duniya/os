#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

// Global variables
int num_processes;
int num_resources;
int available[MAX_RESOURCES];
int maximum[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
int safe_sequence[MAX_PROCESSES];

// Function to check if the system is in safe state
int is_safe() {
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESSES] = {0};
    int i, j, k;
    int count = 0;
    
    // Initialize work array
    for(i = 0; i < num_resources; i++)
        work[i] = available[i];
    
    // Find a process that can be allocated resources
    while(count < num_processes) {
        int found = 0;
        for(i = 0; i < num_processes; i++) {
            if(!finish[i]) {
                int can_allocate = 1;
                
                // Check if all resources can be allocated
                for(j = 0; j < num_resources; j++) {
                    if(need[i][j] > work[j]) {
                        can_allocate = 0;
                        break;
                    }
                }
                
                // If we can allocate resources to this process
                if(can_allocate) {
                    for(k = 0; k < num_resources; k++)
                        work[k] += allocation[i][k];
                    
                    safe_sequence[count] = i;
                    finish[i] = 1;
                    count++;
                    found = 1;
                }
            }
        }
        
        // If no process can be allocated resources, system is unsafe
        if(!found) {
            return 0;
        }
    }
    
    return 1;
}

// Function to print current state
void print_state() {
    int i, j;
    
    printf("\nCurrent System State:\n");
    printf("\nAllocation Matrix:\n");
    for(i = 0; i < num_processes; i++) {
        for(j = 0; j < num_resources; j++)
            printf("%d ", allocation[i][j]);
        printf("\n");
    }
    
    printf("\nMaximum Matrix:\n");
    for(i = 0; i < num_processes; i++) {
        for(j = 0; j < num_resources; j++)
            printf("%d ", maximum[i][j]);
        printf("\n");
    }
    
    printf("\nNeed Matrix:\n");
    for(i = 0; i < num_processes; i++) {
        for(j = 0; j < num_resources; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }
    
    printf("\nAvailable Resources:\n");
    for(i = 0; i < num_resources; i++)
        printf("%d ", available[i]);
    printf("\n");
}

int main() {
    int i, j;
    
    // Get number of processes and resources
    printf("Enter number of processes: ");
    scanf("%d", &num_processes);
    printf("Enter number of resources: ");
    scanf("%d", &num_resources);
    
    // Get available resources
    printf("\nEnter available resources:\n");
    for(i = 0; i < num_resources; i++) {
        printf("Resource %d: ", i);
        scanf("%d", &available[i]);
    }
    
    // Get allocation matrix
    printf("\nEnter allocation matrix:\n");
    for(i = 0; i < num_processes; i++) {
        printf("Process %d:\n", i);
        for(j = 0; j < num_resources; j++) {
            printf("Resource %d: ", j);
            scanf("%d", &allocation[i][j]);
        }
    }
    
    // Get maximum matrix
    printf("\nEnter maximum matrix:\n");
    for(i = 0; i < num_processes; i++) {
        printf("Process %d:\n", i);
        for(j = 0; j < num_resources; j++) {
            printf("Resource %d: ", j);
            scanf("%d", &maximum[i][j]);
        }
    }
    
    // Calculate need matrix
    for(i = 0; i < num_processes; i++)
        for(j = 0; j < num_resources; j++)
            need[i][j] = maximum[i][j] - allocation[i][j];
    
    // Print current state
    print_state();
    
    // Check if system is in safe state
    if(is_safe()) {
        printf("\nSystem is in SAFE state.\n");
        printf("Safe sequence is: ");
        for(i = 0; i < num_processes; i++)
            printf("P%d ", safe_sequence[i]);
        printf("\n");
    } else {
        printf("\nSystem is in UNSAFE state!\n");
    }
    
    return 0;
}
