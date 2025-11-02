#include <stdio.h>
#include <stdlib.h>

#define MAX_CYLINDER 4999
#define MIN_CYLINDER 0
#define NUM_REQUESTS 9

// Function to sort array
void sort(int arr[], int n) {
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// Function to implement SCAN
void scan(int requests[], int n, int head, int prev_head) {
    int total_movement = 0;
    int current = head;
    int sequence[20];  // To store sequence of movements
    int seq_idx = 0;
    
    // Create copy of requests and sort them
    int sorted[NUM_REQUESTS];
    for(int i = 0; i < n; i++) {
        sorted[i] = requests[i];
    }
    sort(sorted, n);
    
    printf("\nSCAN Algorithm:\n");
    printf("Initial head position: %d\n", head);
    printf("Previous head position: %d (Moving towards 4999)\n", prev_head);
    
    // Find the closest cylinder on right of head
    int start_idx = 0;
    while(start_idx < n && sorted[start_idx] <= head) {
        start_idx++;
    }
    
    // Move towards 4999
    printf("\nMovement sequence: %d", head);
    sequence[seq_idx++] = head;
    
    // Service requests from head to 4999
    for(int i = start_idx; i < n; i++) {
        printf(" -> %d", sorted[i]);
        sequence[seq_idx++] = sorted[i];
        total_movement += abs(sorted[i] - current);
        current = sorted[i];
    }
    
    // Move to end
    printf(" -> %d", MAX_CYLINDER);
    sequence[seq_idx++] = MAX_CYLINDER;
    total_movement += abs(MAX_CYLINDER - current);
    current = MAX_CYLINDER;
    
    // Service requests from right to left
    for(int i = start_idx-1; i >= 0; i--) {
        printf(" -> %d", sorted[i]);
        sequence[seq_idx++] = sorted[i];
        total_movement += abs(sorted[i] - current);
        current = sorted[i];
    }
    
    printf("\nTotal head movement: %d cylinders", total_movement);
    printf("\nAverage seek distance: %.2f cylinders\n", (float)total_movement/n);
}

// Function to implement LOOK
void look(int requests[], int n, int head, int prev_head) {
    int total_movement = 0;
    int current = head;
    int sequence[20];  // To store sequence of movements
    int seq_idx = 0;
    
    // Create copy of requests and sort them
    int sorted[NUM_REQUESTS];
    for(int i = 0; i < n; i++) {
        sorted[i] = requests[i];
    }
    sort(sorted, n);
    
    printf("\nLOOK Algorithm:\n");
    printf("Initial head position: %d\n", head);
    printf("Previous head position: %d (Moving towards 4999)\n", prev_head);
    
    // Find the closest cylinder on right of head
    int start_idx = 0;
    while(start_idx < n && sorted[start_idx] <= head) {
        start_idx++;
    }
    
    // Move towards highest request
    printf("\nMovement sequence: %d", head);
    sequence[seq_idx++] = head;
    
    // Service requests from head to highest
    for(int i = start_idx; i < n; i++) {
        printf(" -> %d", sorted[i]);
        sequence[seq_idx++] = sorted[i];
        total_movement += abs(sorted[i] - current);
        current = sorted[i];
    }
    
    // Service requests from right to left
    for(int i = start_idx-1; i >= 0; i--) {
        printf(" -> %d", sorted[i]);
        sequence[seq_idx++] = sorted[i];
        total_movement += abs(sorted[i] - current);
        current = sorted[i];
    }
    
    printf("\nTotal head movement: %d cylinders", total_movement);
    printf("\nAverage seek distance: %.2f cylinders\n", (float)total_movement/n);
}

int main() {
    int head = 143;
    int prev_head = 125;
    int requests[NUM_REQUESTS] = {86, 1470, 913, 1774, 948, 1509, 1022, 1750, 130};
    
    printf("Disk Scheduling Simulation\n");
    printf("Cylinder range: 0-4999\n");
    printf("Request queue: ");
    for(int i = 0; i < NUM_REQUESTS; i++) {
        printf("%d ", requests[i]);
    }
    printf("\n");
    
    // Run both algorithms
    scan(requests, NUM_REQUESTS, head, prev_head);
    printf("\n");
    look(requests, NUM_REQUESTS, head, prev_head);
    
    return 0;
}