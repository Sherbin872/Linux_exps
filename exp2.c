#include <stdio.h>
#include <string.h>

#define N 5

typedef struct {
    int id;
    int burst_time;
    int priority;
    int completion_time;
    int turnaround_time;
    int start_time;
} Process;

// Function to copy original process list into a working array
void copy_processes(Process src[], Process dest[]) {
    for (int i = 0; i < N; i++) {
        dest[i] = src[i];
    }
}

// Function to calculate turnaround time
void calculate_turnaround(Process p[]) {
    for (int i = 0; i < N; i++) {
        p[i].turnaround_time = p[i].completion_time;
    }
}

// FCFS Scheduling
float fcfs(Process original[]) {
    Process p[N];
    copy_processes(original, p);
    
    int time = 0;
    for (int i = 0; i < N; i++) {
        p[i].start_time = time;
        time += p[i].burst_time;
        p[i].completion_time = time;
    }
    calculate_turnaround(p);

    float total = 0;
    for (int i = 0; i < N; i++) {
        total += p[i].turnaround_time;
    }
    return total / N;
}

// SJF Scheduling
float sjf(Process original[]) {
    Process p[N];
    copy_processes(original, p);

    // Sort by burst time
    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            if (p[j].burst_time < p[i].burst_time) {
                Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    int time = 0;
    for (int i = 0; i < N; i++) {
        p[i].start_time = time;
        time += p[i].burst_time;
        p[i].completion_time = time;
    }
    calculate_turnaround(p);

    float total = 0;
    for (int i = 0; i < N; i++) {
        total += p[i].turnaround_time;
    }
    return total / N;
}

// Priority Scheduling (non-preemptive)
float priority_scheduling(Process original[]) {
    Process p[N];
    copy_processes(original, p);

    // Sort by priority (lower number = higher priority)
    for (int i = 0; i < N - 1; i++) {
        for (int j = i + 1; j < N; j++) {
            if (p[j].priority < p[i].priority) {
                Process temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }

    int time = 0;
    for (int i = 0; i < N; i++) {
        p[i].start_time = time;
        time += p[i].burst_time;
        p[i].completion_time = time;
    }
    calculate_turnaround(p);

    float total = 0;
    for (int i = 0; i < N; i++) {
        total += p[i].turnaround_time;
    }
    return total / N;
}

int main() {
    Process processes[N] = {
        {0, 350, 5, 0, 0, 0},
        {1, 125, 2, 0, 0, 0},
        {2, 475, 3, 0, 0, 0},
        {3, 250, 1, 0, 0, 0},
        {4, 75,  3, 0, 0, 0}
    };

    float avg_fcfs = fcfs(processes);
    float avg_sjf = sjf(processes);
    float avg_priority = priority_scheduling(processes);

    printf("Average Turnaround Time:\n");
    printf("FCFS: %.2f\n", avg_fcfs);
    printf("SJF: %.2f\n", avg_sjf);
    printf("Priority Scheduling: %.2f\n", avg_priority);

    if (avg_sjf < avg_fcfs && avg_sjf < avg_priority)
        printf("\nSJF yields the minimum average turnaround time.\n");
    else if (avg_priority < avg_fcfs)
        printf("\nPriority Scheduling yields the minimum average turnaround time.\n");
    else
        printf("\nFCFS yields the minimum average turnaround time.\n");

    return 0;
}
