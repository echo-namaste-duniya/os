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

// Merge function for merge sort
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for(i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for(j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while(i < n1 && j < n2) {
        if(L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while(i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while(j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

// Merge Sort implementation
void mergeSort(int arr[], int l, int r) {
    if(l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Quick Sort implementation
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for(int j = low; j <= high - 1; j++) {
        if(arr[j] < pivot) {
            i++;
            // Swap
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    // Swap
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if(low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
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
        printf("\nChild process (PID: %d) performing Quick Sort\n", getpid());
        printf("Parent PID: %d\n", getppid());
        quickSort(child_arr, 0, n-1);
        printArray(child_arr, n, "Child");
        
        // Demonstrate Orphan state
        printf("\nChild going to sleep to demonstrate orphan state...\n");
        sleep(5);
        printf("Child woke up, Parent PID now: %d\n", getppid());
        exit(0);
    }
    else {
        // Parent process
        printf("\nParent process (PID: %d) performing Merge Sort\n", getpid());
        mergeSort(arr, 0, n-1);
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