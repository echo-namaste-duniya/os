#include <stdio.h>
#include <stdlib.h>

#define REF_STRING_LEN 13  // Length of the reference string including duplicates

// Structure for a page frame
typedef struct {
    int page_number;
    int time_loaded;  // For LRU algorithm
    int next_use;     // For Optimal algorithm
} Frame;

// Function to print current frames
void print_frames(Frame* frames, int num_frames) {
    printf("Frames: ");
    for(int i = 0; i < num_frames; i++) {
        if(frames[i].page_number == -1)
            printf("[ ] ");
        else
            printf("[%d] ", frames[i].page_number);
    }
    printf("\n");
}

// Function to check if a page exists in frames
int page_exists(Frame* frames, int num_frames, int page) {
    for(int i = 0; i < num_frames; i++) {
        if(frames[i].page_number == page)
            return i;
    }
    return -1;
}

// Function to find next use of a page
int find_next_use(int* ref_string, int current_pos, int page, int length) {
    for(int i = current_pos + 1; i < length; i++) {
        if(ref_string[i] == page)
            return i;
    }
    return length; // If page is not used again
}

// Optimal Page Replacement Algorithm
int optimal(int* ref_string, int length, int num_frames) {
    Frame* frames = (Frame*)malloc(num_frames * sizeof(Frame));
    int page_faults = 0;
    
    // Initialize frames
    for(int i = 0; i < num_frames; i++) {
        frames[i].page_number = -1;
    }
    
    printf("\nOptimal Simulation with %d frames:\n", num_frames);
    
    // Process each page reference
    for(int i = 0; i < length; i++) {
        printf("\nReferencing page %d: ", ref_string[i]);
        
        if(page_exists(frames, num_frames, ref_string[i]) == -1) {
            // Find empty frame or page to replace
            int replace_pos = -1;
            int furthest_use = -1;
            
            for(int j = 0; j < num_frames; j++) {
                if(frames[j].page_number == -1) {
                    replace_pos = j;
                    break;
                }
                
                int next = find_next_use(ref_string, i, frames[j].page_number, length);
                if(next > furthest_use) {
                    furthest_use = next;
                    replace_pos = j;
                }
            }
            
            frames[replace_pos].page_number = ref_string[i];
            page_faults++;
            printf("Page Fault! ");
        } else {
            printf("Page Hit! ");
        }
        
        print_frames(frames, num_frames);
    }
    
    free(frames);
    return page_faults;
}

// LRU Page Replacement Algorithm
int lru(int* ref_string, int length, int num_frames) {
    Frame* frames = (Frame*)malloc(num_frames * sizeof(Frame));
    int page_faults = 0;
    int time = 0;
    
    // Initialize frames
    for(int i = 0; i < num_frames; i++) {
        frames[i].page_number = -1;
        frames[i].time_loaded = 0;
    }
    
    printf("\nLRU Simulation with %d frames:\n", num_frames);
    
    // Process each page reference
    for(int i = 0; i < length; i++) {
        printf("\nReferencing page %d: ", ref_string[i]);
        
        int frame_pos = page_exists(frames, num_frames, ref_string[i]);
        if(frame_pos == -1) {
            // Find empty frame or least recently used frame
            int replace_pos = 0;
            for(int j = 1; j < num_frames; j++) {
                if(frames[j].page_number == -1) {
                    replace_pos = j;
                    break;
                }
                if(frames[j].time_loaded < frames[replace_pos].time_loaded) {
                    replace_pos = j;
                }
            }
            
            frames[replace_pos].page_number = ref_string[i];
            frames[replace_pos].time_loaded = time;
            page_faults++;
            printf("Page Fault! ");
        } else {
            frames[frame_pos].time_loaded = time;
            printf("Page Hit! ");
        }
        
        time++;
        print_frames(frames, num_frames);
    }
    
    free(frames);
    return page_faults;
}

int main() {
    // Given reference string: 1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5
    int ref_string[] = {1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5};
    
    printf("Page Reference String:\n");
    for(int i = 0; i < REF_STRING_LEN; i++) {
        printf("%d ", ref_string[i]);
    }
    printf("\n");
    
    // Test with 3 frames
    printf("\n=== Testing with 3 frames ===\n");
    int opt_faults_3 = optimal(ref_string, REF_STRING_LEN, 3);
    int lru_faults_3 = lru(ref_string, REF_STRING_LEN, 3);
    printf("\nResults with 3 frames:\n");
    printf("Optimal: %d page faults\n", opt_faults_3);
    printf("LRU: %d page faults\n", lru_faults_3);
    
    // Test with 4 frames
    printf("\n=== Testing with 4 frames ===\n");
    int opt_faults_4 = optimal(ref_string, REF_STRING_LEN, 4);
    int lru_faults_4 = lru(ref_string, REF_STRING_LEN, 4);
    printf("\nResults with 4 frames:\n");
    printf("Optimal: %d page faults\n", opt_faults_4);
    printf("LRU: %d page faults\n", lru_faults_4);
    
    return 0;
}