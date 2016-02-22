__kernel void firmware(                     
   __global int* input1,                
   __global int* input2,                
   __global int* output,              
   __global int* status,              
   const unsigned int count)           
{
   int i = get_global_id(0); 
if (i==0) {
  *status=0;
}
   if(i < count) {
       output[i] = input1[i] * input2[i];
   }
} 
