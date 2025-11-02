#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void bubbleSort(int arr[], int n) {
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

int main() {
    int n;
    printf("Enter the number of integers: ");
    scanf("%d", &n);

    int *arr = (int *)malloc(n * sizeof(int));
    printf("Enter %d integers:\n", n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Original array: ");
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Sort the array
    bubbleSort(arr, n);

    printf("Sorted array: ");
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    int searchItem;
    printf("Enter number to search: ");
    scanf("%d", &searchItem);

    pid_t pid = fork();

    if(pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }
    else if(pid == 0) {
        // Child process
        // Convert array elements to strings
        char **args = (char **)malloc((n + 3) * sizeof(char *));
        args[0] = "./binary_search";
        
        // Convert n to string
        char nStr[10];
        sprintf(nStr, "%d", n);
        args[1] = strdup(nStr);
        
        // Convert search item to string
        char searchStr[10];
        sprintf(searchStr, "%d", searchItem);
        args[2] = strdup(searchStr);
        
        // Convert array elements to strings
        for(int i = 0; i < n; i++) {
            char *numStr = (char *)malloc(10 * sizeof(char));
            sprintf(numStr, "%d", arr[i]);
            args[i + 3] = numStr;
        }
        args[n + 3] = NULL;

        char *envp[] = { NULL };
        execve("./binary_search", args, envp);
        
        // If execve fails
        printf("execve failed!\n");
        exit(1);
    }
    else {
        // Parent process
        wait(NULL);
        printf("Parent process completed\n");
    }

    free(arr);
    return 0;
}