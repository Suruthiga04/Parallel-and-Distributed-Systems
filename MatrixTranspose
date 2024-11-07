#include<stdio.h>
#include<stdlib.h>
#define row 4
#define col 4

global void transpose(int *a)
{
int x=blockIdx.x;
int y=blockIdx.y;

shared  int p[col*row];
p[y*row+x]=a[col*x+y];
__syncthreads();
a[y*row+x]=p[y*row+x];
}
int main()
{
int a[row][col]={{1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4}};
int res[col][row];
int *da;
cudaEvent_t start,stop;
cudaEventCreate(&start);
cudaEventCreate (&stop);

float elapsedTime;
cudaMalloc((void **)&da,row*col*sizeof(int));
cudaMemcpy(da,&a,row*col*sizeof(int),cudaMemcpyHostToDevice);

dim3 grid(row,col);
printf("Before Transpose:\n");
for(int i=0;i<row;i++)
{
for(int j=0;j<col;j++)
{
printf("%d",a[i][j]);
}
printf("\n");
}
cudaEventRecord(start,0);
transpose<<<grid,1>>>(da);
cudaEventRecord(stop,0);
cudaEventElapsedTime(&elapsedTime,start,stop);
printf("\nTime taken for shared memory transpose:%f\n",elapsedTime);
cudaMemcpy(&res,da,row*col*sizeof(int),cudaMemcpyDeviceToHost);
printf("\nAfter transpose:\n");
for(int i=0;i<row;i++)
{
for(int j=0;j<col;j++)
{
printf("%d",res[i][j]);
}
printf("\n");
}
cudaFree(da);
}
