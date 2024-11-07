#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define ROOT 0
int main(int argc, char** argv) {
int num_processes, process_rank;
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
int local_time;
if (process_rank == ROOT) {
int i;
printf("You have %d processes. Enter the logical clock values for each
process:\n", num_processes);
fflush(stdout);
for (i = 0; i < num_processes; i++) {
if (i == ROOT) {
printf("Enter the logical clock value for process %d: ", ROOT);
fflush(stdout);
scanf("%d", &local_time);
} else {
printf("Enter the logical clock value for process %d: ", i);
fflush(stdout);
int input_time;
scanf("%d", &input_time);
MPI_Send(&input_time, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
}
}
} else {
MPI_Recv(&local_time, 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD,
MPI_STATUS_IGNORE);
}
printf("Process %d has local time: %d\n", process_rank, local_time);
fflush(stdout);
int* local_times = NULL;
if (process_rank == ROOT) {
local_times = (int*)malloc(num_processes * sizeof(int));
}
MPI_Gather(&local_time, 1, MPI_INT, local_times, 1, MPI_INT, ROOT,
MPI_COMM_WORLD);
if (process_rank == ROOT) {
int i, sum = 0;
printf("\nCoordinator (Process %d) has received the following local times:\n",
ROOT);
fflush(stdout);
for (i = 0; i < num_processes; i++) {
printf("Process %d time: %d\n", i, local_times[i]);
sum += local_times[i]-local_time;
} fflush(stdout);
int average_time = sum / num_processes;
printf("\nCoordinator calculated the average time: %d\n", average_time);
fflush(stdout);
int* adjustments = (int*)malloc(num_processes * sizeof(int));
for (i = 0; i < num_processes; i++) {
adjustments[i] = average_time - (local_times[i]-local_time);
printf("Process %d should adjust its time by: %d\n", i, adjustments[i]);
fflush(stdout);
}
for (i = 1; i < num_processes; i++) {
MPI_Send(&adjustments[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
}
local_time += adjustments[ROOT];
printf("Process %d adjusted its time by %d. New local time: %d\n", ROOT,
adjustments[ROOT], local_time);
fflush(stdout);
free(local_times);
free(adjustments);
} else {
int adjustment;
MPI_Recv(&adjustment, 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD,
MPI_STATUS_IGNORE);
local_time += adjustment;
printf("Process %d adjusted its time by %d. New local time: %d\n",
process_rank, adjustment, local_time);
fflush(stdout);
}
MPI_Finalize();
return 0;
}
