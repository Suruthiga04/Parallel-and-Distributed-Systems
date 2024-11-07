#include<stdio.h>
#include<cuda.h>
__device__ int temp[9][9];
__global__ void traverse(int *parent,int *child,int *sibling,int *edge0
,int *edge1,int *succ0,int *succ1,int *position,int *preorder) {
int i=threadIdx.x;
if(parent[edge0[i]]==edge1[i]){
 if(sibling[edge0[i]]!=-1){
 succ0[i]=edge1[i];
 succ1[i]=sibling[edge0[i]];
 }
 else if(parent[edge1[i]]!=-1){
 succ0[i]=edge1[i];
 succ1[i]=parent[edge1[i]];
 }
 else{
 succ0[i]=edge0[i];
 succ1[i]=edge1[i];
 preorder[edge1[i]]=1;
}
}
else{
if(child[edge1[i]]!=-1){
succ0[i]=edge1[i];
 succ1[i]=child[edge1[i]];
 }
 else{
 succ0[i]=edge1[i];
 succ1[i]=edge0[i];
 }
}
if(parent[edge0[i]]==edge1[i]){
 position[i]=0;
}
else{
 position[i]=1;
}
int x;
for(int k=0;k<4;k++){
 x=temp[succ0[i]][succ1[i]];
 position[i]=position[i]+position[x];
 succ0[i]=succ0[x];
 succ1[i]=succ1[x];
}
if(edge0[i]==parent[edge1[i]]){
 preorder[edge1[i]]=9+1-position[i];
}
}
__global__ void initialize(int *edge0,int *edge1){
 for(int i=0;i<16;i++){
 temp[edge0[i]][edge1[i]]=i;
 }
}
int main()
{
char vertices[9]={'a','b','c','d','e','f','g','h','i'};
int parent[9]={-1,0,0,1,1,2,3,3,4};
int child[9]={1,3,5,6,8,-1,-1,-1,-1};
int sibling[9]={-1,2,-1,4,-1,-1,7,-1,-1};
int edge0[16]={0,1,1,3,3,6,3,7,1,4,0,2,4,8,2,5};
int edge1[16]={1,0,3,1,6,3,7,3,4,1,2,0,8,4,5,2};
int succ0[16]; int succ1[16]; int position[16]; int preorder[9];
int *dparent,*dchild,*dsibling,*dedge0,*dedge1,*dsucc0,*dsucc1;
int *dposition,*dpreorder;
cudaMalloc((void**)&dparent,9*sizeof(int));
cudaMalloc((void**)&dchild,9*sizeof(int));
cudaMalloc((void**)&dsibling,9*sizeof(int));
cudaMalloc((void**)&dedge0,16*sizeof(int));
cudaMalloc((void**)&dedge1,16*sizeof(int));
cudaMalloc((void**)&dsucc0,16*sizeof(int));
cudaMalloc((void**)&dsucc1,16*sizeof(int));
cudaMalloc((void**)&dposition,16*sizeof(int));
cudaMalloc((void**)&dpreorder,9*sizeof(int));
cudaMemcpy(dparent,&parent,9*sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpy(dchild,&child,9*sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpy(dsibling,&sibling,9*sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpy(dedge0,&edge0,16*sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpy(dedge1,&edge1,16*sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpy(dsucc0,&succ0,16*sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpy(dsucc1,&succ1,16*sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpy(dposition,&position,16*sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpy(dpreorder,&preorder,9*sizeof(int),cudaMemcpyHostToDevice);
initialize<<<1,1>>>(dedge0,dedge1);
traverse<<<1,16>>>(dparent,dchild,dsibling,dedge0,dedge1,dsucc0,dsucc1,
dposition,dpreorder);
cudaMemcpy(&succ0,dsucc0,16*sizeof(int),cudaMemcpyDeviceToHost);
cudaMemcpy(&succ1,dsucc1,16*sizeof(int),cudaMemcpyDeviceToHost);
cudaMemcpy(&preorder,dpreorder,9*sizeof(int),cudaMemcpyDeviceToHost);
printf("Preorder Traversal numbering to the vertices: \n");
for(int i=0;i<9;i++){
 printf("%c -> %d\n",vertices[i],preorder[i]);
}
cudaFree(dparent);
cudaFree(dchild);
cudaFree(dsibling);
cudaFree(dedge0);
cudaFree(dedge1);
cudaFree(dsucc0);
cudaFree(dsucc1);
cudaFree(dposition);
cudaFree(dpreorder);
return 0;
}
