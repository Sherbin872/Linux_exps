#include <stdio.h>
#include <stdbool.h>

#define N 3 // Number of processes

int main() {
    int total_tape_drives = 13;

    int max[N] = {11, 5, 8};       // Maximum demand of each process
    int alloc[N] = {6, 3, 3};      // Currently allocated
    int need[N];                   // Remaining needs
    int available;                 // Available resources

    // Calculate total allocated
    int total_alloc = 0;
    for (int i = 0; i < N; i++) {
        total_alloc += alloc[i];
    }

    // Calculate available
    available = total_tape_drives - total_alloc;

    // Calculate need = max - alloc
    for (int i = 0; i < N; i++) {
        need[i] = max[i] - alloc[i];
    }

    bool finish[N] = {false};
    int safe_sequence[N];
    int count = 0;

    printf("Initial Available Tape Drives: %d\n", available);

    while (count < N) {
        bool found = false;
        for (int i = 0; i < N; i++) {
            if (!finish[i] && need[i] <= available) {
                // Process can safely execute
                available += alloc[i];
                finish[i] = true;
                safe_sequence[count++] = i;
                found = true;
                break;
            }
        }
        if (!found) {
            break;
        }
    }

    if (count == N) {
        printf("System is in SAFE state.\nSafe sequence: ");
        for (int i = 0; i < N; i++) {
            printf("P%d ", safe_sequence[i] + 1); // +1 for human-readable
        }
        printf("\n");
    } else {
        printf("System is in UNSAFE state!\n");
    }

    return 0;
}
