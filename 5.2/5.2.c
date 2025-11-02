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

// Function to read system state from file
int read_state_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }
    
    // Read number of processes and resources
    fscanf(file, "%d", &num_processes);
    fscanf(file, "%d", &num_resources);
    
    // Skip empty line
    char buffer[100];
    fgets(buffer, sizeof(buffer), file);
    fgets(buffer, sizeof(buffer), file);
    
    // Read allocation matrix
    for(int i = 0; i < num_processes; i++) {
        for(int j = 0; j < num_resources; j++) {
            fscanf(file, "%d", &allocation[i][j]);
        }
    }
    
    // Skip empty line
    fgets(buffer, sizeof(buffer), file);
    fgets(buffer, sizeof(buffer), file);
    
    // Read maximum matrix
    for(int i = 0; i < num_processes; i++) {
        for(int j = 0; j < num_resources; j++) {
            fscanf(file, "%d", &maximum[i][j]);
        }
    }
    
    // Skip empty line
    fgets(buffer, sizeof(buffer), file);
    fgets(buffer, sizeof(buffer), file);
    
    // Read available resources
    for(int i = 0; i < num_resources; i++) {
        fscanf(file, "%d", &available[i]);
    }
    
    fclose(file);
    
    // Calculate need matrix
    for(int i = 0; i < num_processes; i++) {
        for(int j = 0; j < num_resources; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
    
    return 1;
}

// Function to check if the system is in safe state
int is_safe() {
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESSES] = {0};
    int count = 0;
    
    // Initialize work array
    for(int i = 0; i < num_resources; i++)
        work[i] = available[i];
    
    // Find a process that can be allocated resources
    while(count < num_processes) {
        int found = 0;
        for(int i = 0; i < num_processes; i++) {
            if(!finish[i]) {
                int can_allocate = 1;
                
                // Check if all resources can be allocated
                for(int j = 0; j < num_resources; j++) {
                    if(need[i][j] > work[j]) {
                        can_allocate = 0;
                        break;
                    }
                }
                
                // If we can allocate resources to this process
                if(can_allocate) {
                    for(int k = 0; k < num_resources; k++)
                        work[k] += allocation[i][k];
                    
                    safe_sequence[count] = i;
                    finish[i] = 1;
                    count++;
                    found = 1;
                }
            }
        }
        
        if(!found) {
            return 0;  // System is unsafe
        }
    }
    
    return 1;  // System is safe
}

// Function to handle resource request
int request_resources(int process_id, int request[]) {
    // Check if request is valid
    for(int i = 0; i < num_resources; i++) {
        if(request[i] > need[process_id][i]) {
            printf("Error: Process %d is requesting more than its maximum claim\n", process_id);
            return 0;
        }
        if(request[i] > available[i]) {
            printf("Error: Not enough resources available\n");
            return 0;
        }
    }
    
    // Try to allocate resources
    for(int i = 0; i < num_resources; i++) {
        available[i] -= request[i];
        allocation[process_id][i] += request[i];
        need[process_id][i] -= request[i];
    }
    
    // Check if resulting state is safe
    if(is_safe()) {
        printf("Resources allocated to Process %d\n", process_id);
        return 1;
    }
    
    // If unsafe, rollback changes
    for(int i = 0; i < num_resources; i++) {
        available[i] += request[i];
        allocation[process_id][i] -= request[i];
        need[process_id][i] += request[i];
    }
    
    printf("Request denied: resulting state would be unsafe\n");
    return 0;
}

// Function to print current state
void print_state() {
    printf("\nCurrent System State:\n");
    printf("Number of processes: %d\n", num_processes);
    printf("Number of resources: %d\n", num_resources);
    
    printf("\nAllocation Matrix:\n");
    for(int i = 0; i < num_processes; i++) {
        for(int j = 0; j < num_resources; j++)
            printf("%d ", allocation[i][j]);
        printf("\n");
    }
    
    printf("\nMaximum Matrix:\n");
    for(int i = 0; i < num_processes; i++) {
        for(int j = 0; j < num_resources; j++)
            printf("%d ", maximum[i][j]);
        printf("\n");
    }
    
    printf("\nNeed Matrix:\n");
    for(int i = 0; i < num_processes; i++) {
        for(int j = 0; j < num_resources; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }
    
    printf("\nAvailable Resources:\n");
    for(int i = 0; i < num_resources; i++)
        printf("%d ", available[i]);
    printf("\n");
}

int main() {
    // Read initial state from file
    if(!read_state_from_file("state.txt")) {
        return 1;
    }
    
    // Print initial state
    printf("Initial State:\n");
    print_state();
    
    // Check if initial state is safe
    if(is_safe()) {
        printf("\nSystem is in SAFE state.\n");
        printf("Safe sequence is: ");
        for(int i = 0; i < num_processes; i++)
            printf("P%d ", safe_sequence[i]);
        printf("\n");
    } else {
        printf("\nSystem is in UNSAFE state!\n");
        return 1;
    }
    
    // Resource request demonstration
    while(1) {
        int choice;
        printf("\nMenu:\n");
        printf("1. Request resources\n");
        printf("2. Print current state\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        if(choice == 1) {
            int process_id;
            int request[MAX_RESOURCES];
            
            printf("Enter process ID (0-%d): ", num_processes-1);
            scanf("%d", &process_id);
            
            if(process_id < 0 || process_id >= num_processes) {
                printf("Invalid process ID\n");
                continue;
            }
            
            printf("Enter resource request:\n");
            for(int i = 0; i < num_resources; i++) {
                printf("Resource %d: ", i);
                scanf("%d", &request[i]);
            }
            
            request_resources(process_id, request);
        }
        else if(choice == 2) {
            print_state();
        }
        else if(choice == 3) {
            break;
        }
        else {
            printf("Invalid choice!\n");
        }
    }
    
    return 0;
}