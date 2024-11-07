#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#define n 10
_global_ void oddEvenSortKernel(int *a, int phase) {
unsigned int i = threadIdx.x + blockIdx.x * blockDim.x;
unsigned int index1 = 2 * i;
unsigned int index2 = index1 + 1;
if (phase % 2 == 0) { // Odd phase
if (index2 < n && index1 < n) {
if (a[index1] > a[index2]) {
int temp = a[index1];
a[index1] = a[index2];
a[index2] = temp;
}
}
} else { // Even phase
if (index1 + 1 < n && index1 + 2 < n) {
if (a[index1 + 1] > a[index1 + 2]) {
int temp = a[index1 + 1];
a[index1 + 1] = a[index1 + 2];
a[index1 + 2] = temp;
}
}
}
}
int main() {
int a[n] = {6, 9, 5, 7, 4, 2, 1, 8, 0, 3};
int *d_a;
// Allocate GPU memory
cudaMalloc((void**)&d_a, n * sizeof(int));
cudaMemcpy(d_a, a, n * sizeof(int), cudaMemcpyHostToDevice);
// Define number of threads and blocks
int numThreads = n / 2;
int numBlocks = 1;
// Perform Odd-Even Transposition Sort
for (int phase = 0; phase < n; ++phase) {
oddEvenSortKernel<<<numBlocks, numThreads>>>(d_a, phase);
cudaDeviceSynchronize();
}
// Copy the sorted array back to host
cudaMemcpy(a, d_a, n * sizeof(int), cudaMemcpyDeviceToHost);
cudaFree(d_a);
// Print the sorted array
printf("Sorted Order: ");
for (int i = 0; i < n; i++)
printf("%d ", a[i]);
printf("\n");
return 0;
}
