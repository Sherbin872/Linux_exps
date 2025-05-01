#include <stdio.h>
#include <stdbool.h>

#define TIME_QUANTUM 5
#define MAX_QUEUE 10

typedef struct {
    int arrival_time;
    int cpu1;
    int io;
    int cpu2;
    int cpu_time_used;
    int completion_time;
    bool io_done;
    bool cpu1_done;
    bool cpu2_done;
    int state; // 0 = not started, 1 = running, 2 = waiting (I/O), 3 = ready, 4 = done
} Process;

int min(int a, int b) {
    return a < b ? a : b;
}

int is_in_queue(Process* queue[], int front, int rear, Process* p) {
    for (int i = front; i < rear; i++) {
        if (queue[i] == p) return 1;
    }
    return 0;
}

int main() {
    Process p1 = {0, 10, 30, 10, 0, 0, false, false, false, 0};
    Process p2 = {1, 10, 35, 10, 0, 0, false, false, false, 0};

    int time = 0;
    int cpu_busy_time = 0;

    int io1_remaining = 0, io2_remaining = 0;

    Process* queue[MAX_QUEUE];
    int q_front = 0, q_rear = 0;

    while (!(p1.cpu2_done && p2.cpu2_done)) {
        // Add newly arrived processes
        if (time == p1.arrival_time && !p1.cpu1_done && !is_in_queue(queue, q_front, q_rear, &p1)) {
            p1.state = 3;
            queue[q_rear++] = &p1;
        }
        if (time == p2.arrival_time && !p2.cpu1_done && !is_in_queue(queue, q_front, q_rear, &p2)) {
            p2.state = 3;
            queue[q_rear++] = &p2;
        }

        // Handle I/O completion
        if (!p1.io_done && p1.cpu1_done && io1_remaining == 0 && p1.state == 2) {
            p1.io_done = true;
            p1.state = 3;
            if (!is_in_queue(queue, q_front, q_rear, &p1)) queue[q_rear++] = &p1;
        }
        if (!p2.io_done && p2.cpu1_done && io2_remaining == 0 && p2.state == 2) {
            p2.io_done = true;
            p2.state = 3;
            if (!is_in_queue(queue, q_front, q_rear, &p2)) queue[q_rear++] = &p2;
        }

        // Advance I/O in parallel
        if (p1.state == 2 && io1_remaining > 0) io1_remaining--;
        if (p2.state == 2 && io2_remaining > 0) io2_remaining--;

        // Schedule process if any ready
        if (q_front < q_rear) {
            Process* current = queue[q_front++];
            int work_time = 0;

            if (!current->cpu1_done) {
                work_time = min(TIME_QUANTUM, current->cpu1 - current->cpu_time_used);
                current->cpu_time_used += work_time;
                time += work_time;
                cpu_busy_time += work_time;

                if (current->cpu_time_used == current->cpu1) {
                    current->cpu1_done = true;
                    current->state = 2;
                    if (current == &p1) io1_remaining = p1.io;
                    else io2_remaining = p2.io;
                } else {
                    current->state = 3;
                    queue[q_rear++] = current;
                }
            } else if (!current->cpu2_done && current->io_done) {
                int cpu2_progress = current->cpu_time_used - current->cpu1;
                work_time = min(TIME_QUANTUM, current->cpu2 - cpu2_progress);
                current->cpu_time_used += work_time;
                time += work_time;
                cpu_busy_time += work_time;

                if ((current->cpu_time_used - current->cpu1) == current->cpu2) {
                    current->cpu2_done = true;
                    current->completion_time = time;
                    current->state = 4;
                } else {
                    current->state = 3;
                    queue[q_rear++] = current;
                }
            } else {
                time++;
            }
        } else {
            time++;
        }
    }

    int total_time = time;
    float util_p1 = (float)(p1.cpu1 + p1.cpu2) * 100 / total_time;
    float util_p2 = (float)(p2.cpu1 + p2.cpu2) * 100 / total_time;
    float total_util = (float)cpu_busy_time * 100 / total_time;

    printf("CPU Utilization of P1: %.2f%%\n", util_p1);
    printf("CPU Utilization of P2: %.2f%%\n", util_p2);
    printf("Total CPU Utilization: %.2f%%\n", total_util);

    return 0;
}
