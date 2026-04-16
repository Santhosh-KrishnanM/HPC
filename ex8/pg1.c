#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

int main(){
   int n, i, j;
   printf("3. OpenMP program to perform mirror matrix using parallel for\n");
   printf("Enter no. of elements: ");
   scanf("%d", &n);
   int A[n][n], res[n][n];
   printf("Enter matrix A:\n");
   for(i = 0; i < n; i++){
      for(j = 0; j < n; j++)
	 scanf("%d", &A[i][j]);
   }
#pragma omp parallel for 
   for(i = 0; i < n; i++){
      for(j = 0; j < n; j++)
	 res[i][j] = A[i][n-1-j];
   }

   printf("\n");
   for(i = 0; i < n; i++){
      for(j = 0; j < n; j++)
	 printf("%d ", A[i][j]);
      printf("\n");
   }

   printf("Mirror Matrix:\n");
   for(i = 0; i < n; i++){
      for(j = 0; j < n; j++)
	 printf("%d ", res[i][j]);
      printf("\n");
   }
   return 0;
}
