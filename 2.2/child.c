#include <stdio.h>
#include <stdlib.h>

int binarySearch(int arr[], int n, int key) {
    int l = 0, r = n - 1;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (arr[mid] == key)
            return mid;
        else if (arr[mid] < key)
            l = mid + 1;
        else
            r = mid - 1;
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("No data received from parent.\n");
        exit(1);
    }

    int n = argc - 1;
    int arr[n];
    for (int i = 0; i < n; i++)
        arr[i] = atoi(argv[i + 1]);

    printf("Child received sorted array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    int key;
    printf("Enter element to search: ");
    scanf("%d", &key);

    int result = binarySearch(arr, n, key);

    if (result != -1)
        printf("Element %d found at position %d\n", key, result + 1);
    else
        printf("Element %d not found.\n", key);

    return 0;
}
