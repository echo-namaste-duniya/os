#include <stdio.h>
#include <stdlib.h>

int binarySearch(int arr[], int l, int r, int x) {
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (arr[m] == x)
            return m;
        if (arr[m] < x)
            l = m + 1;
        else
            r = m - 1;
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if(argc < 4) {
        printf("Usage: %s n searchItem num1 num2 ... numN\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int searchItem = atoi(argv[2]);
    int *arr = (int *)malloc(n * sizeof(int));

    // Convert command line arguments to integers
    for(int i = 0; i < n; i++) {
        arr[i] = atoi(argv[i + 3]);
    }

    printf("Binary Search Program\n");
    printf("Searching for: %d\n", searchItem);

    int result = binarySearch(arr, 0, n-1, searchItem);
    if(result == -1)
        printf("Element %d not found in array\n", searchItem);
    else
        printf("Element %d found at index %d\n", searchItem, result);

    free(arr);
    return 0;
}