#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void display_frame(int frame_count,int frames[]){
    printf("\nFrames: ");
        for (int k = 0; k < frame_count; k++) {
            if (frames[k] == -1)
                printf(" - ");
            else
                printf("%d ", frames[k]);
        }
}

void fifo(int frame_count, int frames[], int n, int pages[]) {
    int faults = 0;
    int index = 0;
    for (int i = 0; i <frame_count; i++) {
        frames[i] = -1; // initialize empty frames
    }
    for (int i = 0; i < n; i++) {
        int curr = pages[i];
        int found = 0;

        // Check if page already exists in frame
        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == curr) {
                found = 1;
                printf("\nPage %d Already Exists",curr);

                break;
            }
        }

        // Page not found → Page fault
        if (found == 0) {
            printf("\nPage Fault Occurred for Page %d", curr);
            faults++;
            frames[index] = curr;
            index = (index + 1) % frame_count;
        }

        // Display current frame contents
        display_frame(frame_count,frames);
    }

    printf("\n\nTotal Page Faults in FIFO: %d\n", faults);
}


void lru(int frame_count, int frames[], int n, int pages[]) {
    int faults = 0;
    int *recent = (int *)malloc(frame_count * sizeof(int));

    for (int i = 0; i < frame_count; i++) {
        frames[i] = -1;
        recent[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        int curr = pages[i];
        int found = 0;

        // Check if page already exists
        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == curr) {
                found = 1;
                printf("\nPage %d Already Exists",curr);

                recent[j] = i; // update last used index
                break;
            }
        }

        // If not found → page fault
        if (found == 0) {
            faults++;

            int replace_index = -1;

            // Check for empty frame first
            for (int j = 0; j < frame_count; j++) {
                if (frames[j] == -1) {
                    replace_index = j;
                    break;
                }
            }

            // If no empty frame, find least recently used
            if (replace_index == -1) {
                int lru_index = 0;
                for (int j = 1; j < frame_count; j++) {
                    if (recent[j] < recent[lru_index]) {
                        lru_index = j;
                    }
                }
                replace_index = lru_index;
            }

            // Replace page
            frames[replace_index] = curr;
            recent[replace_index] = i;

            printf("\nPage Fault Occurred for Page %d", curr);
        }

        // Display frame content
        display_frame(frame_count,frames);
    }

    printf("\n\nTotal Page Faults in LRU: %d\n", faults);
    free(recent);
}

void optimal(int frame_count, int frames[], int n, int pages[]) {
    int faults = 0;

    for (int i = 0; i < frame_count; i++) {
        frames[i] = -1; 
    }

    for (int i = 0; i < n; i++) {
        int curr = pages[i];
        int found = 0;

        // Check if page already exists in frame
        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == curr) {
                printf("\nPage %d Already Exists",curr);
                found = 1;
                break;
            }
        }

        // Page not found → Page fault
        if (found == 0) {
            faults++;
            int replace_index = -1;

            // If there's an empty frame, use it
            for (int j = 0; j < frame_count; j++) {
                if (frames[j] == -1) {
                    replace_index = j;
                    break;
                }
            }

            // If no empty frame, find the optimal page to replace
            if (replace_index == -1) {
                int farthest = -1;
                int farthest_index = -1;

                // Check each frame's next use
                for (int j = 0; j < frame_count; j++) {
                    int next_use = -1;
                    for (int k = i + 1; k < n; k++) {
                        if (pages[k] == frames[j]) {
                            next_use = k;
                            break;
                        }
                    }

                    // If page not used in future, replace immediately
                    if (next_use == -1) {
                        farthest_index = j;
                        break;
                    }

                    // Find page used farthest in future
                    if (next_use > farthest) {
                        farthest = next_use;
                        farthest_index = j;
                    }
                }

                replace_index = farthest_index;
            }

            printf("\nPage Fault Occurred for Page %d", curr);
            frames[replace_index] = curr;
        }

        display_frame(frame_count, frames);
    }

    printf("\n\nTotal Page Faults in Optimal: %d\n", faults);
}

void generate_pages(int pages[], int n) {
    srand(time(0)); // Seed random number generator
    for (int i = 0; i < n; i++) {
        pages[i] = rand() % 10; // Pages between 0 and 9
    }
}

int main() {
    printf("Enter the size of page reference string: ");
    int n;
    scanf("%d", &n);

    int *pages = (int *)malloc(n * sizeof(int));
    generate_pages(pages, n);

    printf("\nGenerated Page Reference String: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", pages[i]);
    }
    printf("\n");

    printf("\nEnter the number of frames: ");
    int frames_count;
    scanf("%d", &frames_count);

    int *frames = (int *)malloc(frames_count * sizeof(int));
    fifo(frames_count, frames, n, pages);
    lru(frames_count, frames, n, pages);
    optimal(frames_count, frames, n, pages);

    free(pages);
    free(frames);
    return 0;
}
