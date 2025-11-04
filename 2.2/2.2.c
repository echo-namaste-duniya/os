#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
        }
    }
}

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    bubbleSort(arr, n);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        char *args[n + 3]; // n sorted numbers + program name + search element + NULL
        args[0] = "./child";

        // Convert sorted numbers to strings
        for (int i = 0; i < n; i++) {
            char *num = malloc(10);
            sprintf(num, "%d", arr[i]);
            args[i + 1] = num;
        }

        // Add NULL terminator
        args[n + 1] = NULL;

        execve("./child", args, NULL);
        perror("execve failed");
        exit(1);
    } 
    else if (pid > 0) {
        wait(NULL);
        printf("Parent: Child finished execution.\n");
    } 
    else {
        perror("fork failed");
        exit(1);
    }

    return 0;
}
