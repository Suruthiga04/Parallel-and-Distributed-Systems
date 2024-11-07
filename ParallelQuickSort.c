#include <stdio.h>
#define N 8
struct bound
{
   int low;
   int high;
};
device struct bound stack[10];
device int top = -1;
device void push(int l, int h )
{
   if(top < N+1)
   {
     struct bound b;
     b.low = l;
     b.high = h;
     stack[++top] = b;
   }
  //printf("\npushing ->%d, %d", stack[top].low, stack[top].high);
}
device struct bound pop(int pid)
{
       struct bound popVal;
       popVal = stack[top];
       top--;
       return popVal;
}
global void initialize()
{
    push(0, N-1);
}
device void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
device void printarr(int *arr)
{
    printf("\n");
    for(int i = 0; i < N; i++)
      printf(" %d\t ", arr[i]);
    printf( "\n");
}
device int partition(int *a, int start, int end)
{
     int pivot=a[start],p1=start+1,i,temp;
                                                      for(i=start+1;i<=end;i++){
      if(a[i]<pivot){
          if(i!=p1){
            temp=a[p1];
            a[p1]=a[i];
            a[i]=temp;
          }
          p1++;
      }
    }

    a[start]=a[p1-1];
    a[p1-1]=pivot;
    //printarr(a);
    return p1-1;
}
device int sorted  = 0;
global void quicksort(int *arr)
{

  int pid = threadIdx.x;
  struct bound b;
  while(sorted < N)
  {
        if(stack[top].low == pid)
        {
                b = pop(pid);
        }
        while(b.low < b.high)
        {
                //printf("\n%d = %d, %d ", pid,  b.low, b.high );
                int med = partition(arr, b.low, b.high);
                //printf("\n median = %d " , med);
      if(med+1 < b.high)
                {
                        push(med+1, b.high);
                }
      b.high = med-1;
      if(b.low == b.high)
                        atomicAdd(&sorted, 2);
      else
            atomicAdd(&sorted, 1);

     }
    if(b.low==b.high)
        atomicAdd(&sorted,1);
    __syncthreads();

  }
  __syncthreads();
  //printarr(arr);
  //printf("\nfinal-> %d", sorted);
}
int main(){

   int arr[N] = {4,8,7,5,1,2,3,6};
   int *darr;
   cudaMalloc((int**)&darr, N*sizeof(int));
   initialize<<<1,1>>>();
   cudaMemcpy(darr, arr, N*sizeof(int),cudaMemcpyHostToDevice);
   quicksort<<<1,N>>>(darr);
   cudaDeviceSynchronize();
   cudaMemcpy(arr, darr, N*sizeof(int), cudaMemcpyDeviceToHost);

   printf("\nQuicksort result: ");
   for(int i = 0; i < N; i++){
      printf("%d, ", arr[i]);
   }
}
