#include <mpi.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> // For sleep function
#define REQUEST 1
#define REPLY 2
#define RELEASE 3
#define MAX_PROCESSES 10
int timestamp = 0;
int num_processes, process_rank;
bool requesting = false;
bool in_critical_section = false;
int replies_count = 0;
bool deferred_reply[MAX_PROCESSES];
void send_message(int dest, int tag) {
if (dest >= 0 && dest < num_processes) {
MPI_Send(&timestamp, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
}
}
void receive_message(int* recv_timestamp, int* source, int* tag) {
MPI_Status status;
MPI_Recv(recv_timestamp, 1, MPI_INT, MPI_ANY_SOURCE,
MPI_ANY_TAG, MPI_COMM_WORLD, &status);
*source = status.MPI_SOURCE;
*tag = status.MPI_TAG;
}
void handle_request(int source, int recv_timestamp) {
bool grant_permission = false;
if (!in_critical_section && !requesting) {
grant_permission = true;
} else if (recv_timestamp < timestamp || (recv_timestamp == timestamp &&
source < process_rank)) {
grant_permission = true;
}
if (grant_permission) {
send_message(source, REPLY);
} else {
deferred_reply[source] = true;
}
}
void handle_reply() {
replies_count++;
if (replies_count == num_processes - 1) {
in_critical_section = true;
printf("Process %d in critical section\n", process_rank);
sleep(1); // Simulate time spent in the critical section
}
}
void handle_release(int source) {
printf("Process %d received RELEASE from process %d\n", process_rank,
source);
}
void request_critical_section() {
requesting = true;
timestamp++;
replies_count = 0;
int i;
for (i = 0; i < num_processes; i++) {
if (i != process_rank) {
send_message(i, REQUEST);
}
}
}
void release_critical_section() {
printf("Process %d releasing critical section\n", process_rank);
in_critical_section = false;
requesting = false;
int i;
for (i = 0; i < num_processes; i++) {
if (deferred_reply[i]) {
send_message(i, REPLY);
deferred_reply[i] = false;
}
}
for (i = 0; i < num_processes; i++) {
if (i != process_rank) {
send_message(i, RELEASE);
}
}
}
int main(int argc, char** argv) {
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
int i;
for (i = 0; i < MAX_PROCESSES; i++) {
deferred_reply[i] = false;
}
if (process_rank == 0) {
sleep(1); // Process 0 starts first
}
request_critical_section();
while (true) {
int recv_timestamp, tag, source;
receive_message(&recv_timestamp, &source, &tag);
timestamp = (timestamp > recv_timestamp) ? timestamp + 1 :
recv_timestamp + 1;
switch (tag) {
case REQUEST:
printf("Process %d received REQUEST from process %d\n",
process_rank, source);
handle_request(source, recv_timestamp);
break;
case REPLY:
printf("Process %d received OK from process %d\n", process_rank,
source);
handle_reply();
break;
case RELEASE:
handle_release(source);
break;
}
if (in_critical_section) {
release_critical_section();
break;
}
}
MPI_Finalize();
return 0;
}
