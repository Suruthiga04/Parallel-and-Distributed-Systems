#include<stdio.h>
#include<stdlib.h>
__global__ void mult(int *a,  int *b, int *c, int n){
int tid = threadIdx.x;
if(tid < n){
c[tid] = a[tid]*b[tid];
}
}
__global__ void add(int *c, int n){
int tid = threadIdx.x;
for(int s = 1;s < n;s=s*2){
if(tid % (2*s) == 0 && (tid+s)<n){
c[tid] = c[tid] + c[tid + s];
}
__syncthreads();
}
}
int main(){
int a[4] = {1,2,3,4};
int b[4] = {1,2,3,4};
int c[4]={0,0,0,0};

int n = 4;
int *da, *db, *dc;

cudaMalloc(&da, 4*sizeof(int));
cudaMalloc(&db, 4*sizeof(int));
cudaMalloc(&dc, 4*sizeof(int));

cudaMemcpy(da, a, 4*sizeof(int), cudaMemcpyHostToDevice);
cudaMemcpy(db, b, 4*sizeof(int), cudaMemcpyHostToDevice);
mult<<<1,4>>>(da , db , dc, n);
add<<<1,4>>>(dc,n);
cudaMemcpy(c, dc, 4*sizeof(int),  cudaMemcpyDeviceToHost);
printf("%d\n",c[0]);

cudaFree(da);
cudaFree(db);
cudaFree(dc);
return 0;
}
