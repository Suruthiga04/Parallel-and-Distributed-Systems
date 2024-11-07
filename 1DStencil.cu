#include <stdio.h>
#include <cuda.h>
__constant__ int stencil[] = {1, 2, 3,4,5};
//constant stencil array size=radius*2+1

__global__ void stencil_1D(int *input, int *output, int n, int radius) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int sum = 0;
    int i=0;
    
    for (int offset = -radius; offset <= radius; ++offset) {
        int index = idx + offset;
        if (index >= 0 && index < n) {
            sum += input[index]*stencil[i];
        }
        i++;
    }

    if (idx < n) {
        output[idx] = sum;
    }
}

int main() {
    const int n = 7;
    const int radius = 2;
    int h_input[n] = {5, 2, 1, 9, 2, 3, 6};
    int h_output[n];

    int *d_input, *d_output;

    
    cudaMalloc((void**)&d_input, n * sizeof(int));
    cudaMalloc((void**)&d_output, n * sizeof(int));

    
    cudaMemcpy(d_input, h_input, n * sizeof(int), cudaMemcpyHostToDevice);

    
    int blockSize = 256;
    int gridSize = (n + blockSize - 1) / blockSize;

    
    stencil_1D<<<gridSize, blockSize>>>(d_input, d_output, n, radius);

    cudaMemcpy(h_output, d_output, n * sizeof(int), cudaMemcpyDeviceToHost);

    
    printf("Input:  ");
    for (int i = 0; i < n; ++i) {
        printf("%d ", h_input[i]);
    }
    printf("\nOutput: ");
    for (int i = 0; i < n; ++i) {
        printf("%d ", h_output[i]);
    }
    printf("\n");

   
    cudaFree(d_input);
    cudaFree(d_output);

    return 0;
}
