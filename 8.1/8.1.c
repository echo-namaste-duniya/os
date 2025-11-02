#include <stdio.h>
#include <stdlib.h>

#define MAX_CYLINDER 499
#define MIN_CYLINDER 0
#define NUM_REQUESTS 10

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

// Function to implement C-SCAN
void cscan(int requests[], int n, int head) {
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
    
    printf("\nC-SCAN Algorithm:\n");
    printf("Initial head position: %d\n", head);
    
    // Find the closest cylinder on right of head
    int start_idx = 0;
    while(start_idx < n && sorted[start_idx] <= head) {
        start_idx++;
    }
    
    // Move towards 499
    printf("\nMovement sequence: %d", head);
    sequence[seq_idx++] = head;
    
    // Service requests from head to 499
    for(int i = start_idx; i < n; i++) {
        printf(" -> %d", sorted[i]);
        sequence[seq_idx++] = sorted[i];
        total_movement += abs(sorted[i] - current);
        current = sorted[i];
    }
    
    if(start_idx != n) {
        // Move to end cylinder
        printf(" -> %d", MAX_CYLINDER);
        sequence[seq_idx++] = MAX_CYLINDER;
        total_movement += abs(MAX_CYLINDER - current);
        
        // Move to beginning
        printf(" -> %d", MIN_CYLINDER);
        sequence[seq_idx++] = MIN_CYLINDER;
        total_movement += MAX_CYLINDER;  // Add movement from max to min
        current = MIN_CYLINDER;
        
        // Service remaining requests from 0 to head
        for(int i = 0; i < start_idx; i++) {
            printf(" -> %d", sorted[i]);
            sequence[seq_idx++] = sorted[i];
            total_movement += abs(sorted[i] - current);
            current = sorted[i];
        }
    }
    
    printf("\nTotal head movement: %d cylinders", total_movement);
    printf("\nAverage seek distance: %.2f cylinders\n", (float)total_movement/n);
}

// Function to implement C-LOOK
void clook(int requests[], int n, int head) {
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
    
    printf("\nC-LOOK Algorithm:\n");
    printf("Initial head position: %d\n", head);
    
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
    
    if(start_idx != n) {
        // Service remaining requests from lowest to head
        for(int i = 0; i < start_idx; i++) {
            printf(" -> %d", sorted[i]);
            sequence[seq_idx++] = sorted[i];
            total_movement += abs(sorted[i] - current);
            current = sorted[i];
        }
    }
    
    printf("\nTotal head movement: %d cylinders", total_movement);
    printf("\nAverage seek distance: %.2f cylinders\n", (float)total_movement/n);
}

int main() {
    int head = 85;
    int requests[NUM_REQUESTS] = {10, 229, 39, 400, 18, 145, 120, 480, 20, 250};
    
    printf("Disk Scheduling Simulation\n");
    printf("Cylinder range: 0-499\n");
    printf("Request queue: ");
    for(int i = 0; i < NUM_REQUESTS; i++) {
        printf("%d ", requests[i]);
    }
    printf("\n");
    
    // Run both algorithms
    cscan(requests, NUM_REQUESTS, head);
    printf("\n");
    clook(requests, NUM_REQUESTS, head);
    
    return 0;
}