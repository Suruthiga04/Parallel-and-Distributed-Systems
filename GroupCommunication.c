#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv) {
int num_processes, process_rank;
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
int array_size = num_processes; // Set array size equal to the number of
processes
char message[100];
if (process_rank == 0) {
printf("Enter a message to broadcast: ");
fflush(stdout); // Force the output to be displayed immediately
fgets(message, 100, stdin);
printf("Array size set to number of processes: %d\n", array_size);
printf("Enter %d elements for the array:\n", array_size);
fflush(stdout); // Force the output to be displayed immediately
}
MPI_Bcast(&array_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
int numbers[array_size]; // Declare the numbers array with size equal to
number of processes
if (process_rank == 0) {
int i;
for (i = 0; i < array_size; i++) {
scanf("%d", &numbers[i]);
}
}
MPI_Bcast(message, 100, MPI_CHAR, 0, MPI_COMM_WORLD);
printf("Process %d received message: %s\n", process_rank, message);
int recv_number;
MPI_Scatter(numbers, 1, MPI_INT, &recv_number, 1, MPI_INT, 0,
MPI_COMM_WORLD);
int result = recv_number * recv_number;
printf("Process %d received %d and computed its square: %d\n",
process_rank, recv_number, result);
int gathered_results[array_size];
MPI_Gather(&result, 1, MPI_INT, gathered_results, 1, MPI_INT, 0,
MPI_COMM_WORLD);
if (process_rank == 0) {
printf("Gathered results: ");
int i;
for (i = 0; i < array_size; i++) {
printf("%d ", gathered_results[i]);
}
printf("\n");
}
int sum_of_squares;
MPI_Reduce(&result, &sum_of_squares, 1, MPI_INT, MPI_SUM, 0,
MPI_COMM_WORLD);
if (process_rank == 0) {
printf("Sum of squares: %d\n", sum_of_squares);
}
MPI_Finalize();
return 0;
}
EXP
