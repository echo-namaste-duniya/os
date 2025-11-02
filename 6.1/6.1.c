#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FRAMES 7
#define MIN_FRAMES 1
#define PAGE_RANGE 10  // Pages from 0 to 9
#define REF_STRING_LEN 20

// Structure for a page frame
typedef struct {
    int page_number;
    int time_loaded;  // For LRU algorithm
} Frame;

// Function to generate random page reference string
void generate_reference_string(int* ref_string, int length) {
    for(int i = 0; i < length; i++) {
        ref_string[i] = rand() % PAGE_RANGE;
    }
}

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

// FIFO Page Replacement Algorithm
int fifo(int* ref_string, int length, int num_frames) {
    Frame* frames = (Frame*)malloc(num_frames * sizeof(Frame));
    int page_faults = 0;
    int current_position = 0;
    
    // Initialize frames
    for(int i = 0; i < num_frames; i++) {
        frames[i].page_number = -1;
        frames[i].time_loaded = 0;
    }
    
    printf("\nFIFO Simulation with %d frames:\n", num_frames);
    
    // Process each page reference
    for(int i = 0; i < length; i++) {
        printf("\nReferencing page %d: ", ref_string[i]);
        
        // Check if page already exists
        if(page_exists(frames, num_frames, ref_string[i]) == -1) {
            // Page fault
            frames[current_position].page_number = ref_string[i];
            current_position = (current_position + 1) % num_frames;
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
            // Page fault - find least recently used frame
            int lru_pos = 0;
            for(int j = 1; j < num_frames; j++) {
                if(frames[j].time_loaded < frames[lru_pos].time_loaded)
                    lru_pos = j;
            }
            
            frames[lru_pos].page_number = ref_string[i];
            frames[lru_pos].time_loaded = time;
            page_faults++;
            printf("Page Fault! ");
        } else {
            // Update time for referenced page
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
    int ref_string[REF_STRING_LEN];
    srand(time(NULL));
    
    // Generate random reference string
    generate_reference_string(ref_string, REF_STRING_LEN);
    
    // Print reference string
    printf("Generated Page Reference String:\n");
    for(int i = 0; i < REF_STRING_LEN; i++) {
        printf("%d ", ref_string[i]);
    }
    printf("\n\n");
    
    // Test with different number of frames
    printf("Page Fault Analysis:\n");
    printf("Frames\tFIFO Faults\tLRU Faults\n");
    
    for(int frames = MIN_FRAMES; frames <= MAX_FRAMES; frames++) {
        int fifo_faults = fifo(ref_string, REF_STRING_LEN, frames);
        int lru_faults = lru(ref_string, REF_STRING_LEN, frames);
        printf("\n%d\t%d\t\t%d\n", frames, fifo_faults, lru_faults);
    }
    
    return 0;
}