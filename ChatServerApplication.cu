#include <mpi.h>
#include <stdio.h>
#include <string.h>
#define MAX_MESSAGE_LENGTH 1024
int main(int argc, char** argv) {
int num_processes, process_rank;
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
if (process_rank == 0) {
char message[MAX_MESSAGE_LENGTH];
printf("\nEnter a message: ");
fflush(stdout); // Flush the output buffer to display the prompt immediately
fgets(message, MAX_MESSAGE_LENGTH, stdin);
MPI_Bcast(message, MAX_MESSAGE_LENGTH, MPI_CHAR, 0,
MPI_COMM_WORLD);
} else {
char message[MAX_MESSAGE_LENGTH];
MPI_Bcast(message, MAX_MESSAGE_LENGTH, MPI_CHAR, 0,
MPI_COMM_WORLD);
printf("\nReceived message from root process: %s", message);
}
int i;
for (i = 0; i < num_processes; i++) {
if (process_rank != i) {
char send_message[MAX_MESSAGE_LENGTH];
sprintf(send_message, "Hello from process %d!", process_rank);
MPI_Send(send_message, MAX_MESSAGE_LENGTH, MPI_CHAR, i, 0,
MPI_COMM_WORLD);
char recv_message[MAX_MESSAGE_LENGTH];
MPI_Recv(recv_message, MAX_MESSAGE_LENGTH, MPI_CHAR, i, 0,
MPI_COMM_WORLD, MPI_STATUS_IGNORE);
printf("\nReceived message from process %d: %s\n", i, recv_message);
}
}
MPI_Finalize();
return 0;
}
