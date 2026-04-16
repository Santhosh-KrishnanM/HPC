#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main(){
   int n, i;

   printf("4. OpenMP program to find the running minimum of the given array\n");
   printf("Enter no. of elements: ");
   scanf("%d", &n);

   int a[n], res[n];

   printf("Enter array: ");
   for(i = 0; i < n; i++){
      scanf("%d", &a[i]);
   }

   //#pragma omp parallel for
   for(i = 0; i < n; i++){
      res[i] = a[i];
   }
  
   //#pragma omp parallel for
   for(int step = 1; step < n; step *= 2){
      #pragma omp parallel for
      for(i = step; i < n; i++){
         if(res[i - step] < res[i]){
            res[i] = res[i - step];
         }
      }
   }

   printf("Minimum value: %d\n", res[n-1]);

   printf("Running minimum array:\n");
   for(i = 0; i < n; i++)
      printf("%d ", res[i]);

   printf("\n");
   return 0;
}
