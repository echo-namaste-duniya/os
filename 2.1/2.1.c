#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// Function to print array
void printArray(int arr[], int n, char *process) {
    printf("\n%s: Sorted array: ", process);
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Bubble Sort implementation
void bubbleSort(int arr[], int n) {
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(arr[j] > arr[j+1]) {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// Insertion Sort implementation
void insertionSort(int arr[], int n) {
    for(int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while(j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

int main() {
    int n;
    printf("Enter the number of integers: ");
    scanf("%d", &n);

    int *arr = (int *)malloc(n * sizeof(int));
    printf("Enter %d integers:\n", n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Create a copy of array for child process
    int *child_arr = (int *)malloc(n * sizeof(int));
    for(int i = 0; i < n; i++) {
        child_arr[i] = arr[i];
    }

    pid_t pid = fork();

    if(pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }
    else if(pid == 0) {
        // Child process
        printf("\nChild process (PID: %d) performing Insertion Sort\n", getpid());
        printf("Parent PID: %d\n", getppid());
        insertionSort(child_arr, n);
        printArray(child_arr, n, "Child");
        
        // Demonstrate Orphan state
        printf("\nChild going to sleep to demonstrate orphan state...\n");
        sleep(5);
        printf("Child woke up, Parent PID now: %d\n", getppid());
        exit(0);
    }
    else {
        // Parent process
        printf("\nParent process (PID: %d) performing Bubble Sort\n", getpid());
        bubbleSort(arr, n);
        printArray(arr, n, "Parent");

        // Demonstrate Zombie state
        printf("\nParent sleeping to demonstrate zombie state...\n");
        sleep(2);
        
        // Wait for child to complete
        wait(NULL);
        printf("Parent process completed\n");
    }

    free(arr);
    free(child_arr);
    return 0;
}