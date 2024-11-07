#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
MPI_Init(&argc, &argv);
int rank, size;
int leader;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
int* uid = malloc(size * sizeof(int));
int* token = malloc(size * sizeof(int));
uid[rank] = rank * 100 + rank;
if (rank == 0) {
token[rank] = uid[rank];
}
if (rank != 0) {
MPI_Recv(token, size, MPI_INT, rank - 1, 0, MPI_COMM_WORLD,
MPI_STATUS_IGNORE);
printf("Process %d received token from process %d\n", rank, rank - 1);
token[rank] = uid[rank];
}
MPI_Send(token, size, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
if (rank == 0) {
MPI_Recv(token, size, MPI_INT, size - 1, 0, MPI_COMM_WORLD,
MPI_STATUS_IGNORE);
printf("Process %d received token from process %d\n", rank, size - 1);
int max = token[0];
leader = 0;
int i;
for (i = 1; i < size; i++) {
if (token[i] > max) {
max = token[i];
leader = i;
}
}
MPI_Send(&leader, 1, MPI_INT, (rank + 1) % size, 1,
MPI_COMM_WORLD);
}
if (rank != 0) {
MPI_Recv(&leader, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD,
MPI_STATUS_IGNORE);
printf("Process %d received leader information from process %d, Leader is
%d\n", rank, rank - 1, leader);
MPI_Send(&leader, 1, MPI_INT, (rank + 1) % size, 1,
MPI_COMM_WORLD);
}
if (rank == 0) {
MPI_Recv(&leader, 1, MPI_INT, size - 1, 1, MPI_COMM_WORLD,
MPI_STATUS_IGNORE);
printf("Process %d received leader information from process %d, Leader is
%d\n", rank, size - 1, leader);
}
free(uid);
free(token);
MPI_Finalize();
return 0;
}
