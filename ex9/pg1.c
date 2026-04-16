#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main(){
   int n,k,i,j;
   printf("\n2.a.Sum of all elements divisible by k in a matrix\n");
   printf("Enter matrix size: ");
   scanf("%d", &n);
   int a[n][n];
   printf("Enter value of k: ");
   scanf("%d", &k);
   for(i = 0; i < n; i++){
      for(j = 0; j < n; j++)
         scanf("%d", &a[i][j]);
   }
   printf("\nDivisible values: ");
   int sum = 0;
#pragma omp parallel for reduction(+:sum)
   for(i = 0; i < n; i++){
      for(j = 0; j < n; j++){
	 if(a[i][j] % k == 0){
	    sum += a[i][j];
	    printf("%d ", a[i][j]);
	 }
      }
   }

   printf("\nSum: %d\n", sum);
   return 0;
}
