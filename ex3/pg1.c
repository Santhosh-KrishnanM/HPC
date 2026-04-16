#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<time.h>
#define MAX 100

int main(int argc, char *argv[]){
   int comm_sz, my_rank = 0, num, sfreq = 0, tfreq, i, n,N;
   int arr[100];
   int *parr;
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   //n = 12 / comm_sz;
   //parr = (int*)malloc(n * sizeof(int));   
   if(my_rank == 0){
      srand(time(0));
      printf("Enter the no. of elements: ");
      fflush(stdout);
      scanf("%d", &N);
      if(N > 20){
         for(i = 0; i < N; i++)
            arr[i] = rand() % N;         
      }else{
	 for(i = 0; i < N; i++){
            arr[i] = rand() % N;
	    printf("%d ", arr[i]);
         }
   	 printf("\n");
      }
      printf("Enter the value to find: ");
      fflush(stdout);
      scanf("%d", &num);
      if(N % comm_sz != 0){
	      printf("\nThe given elements are not equally allocated to the processes\n");
	      fflush(stdout);
	      MPI_Abort(MPI_COMM_WORLD,1);
      }
      n = N / comm_sz;
   } 
   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
   //n = N / comm_sz;
   parr = (int*)malloc(n * sizeof(int));    
   MPI_Scatter(arr, n, MPI_INT, parr, n, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&num, 1, MPI_INT, 0, MPI_COMM_WORLD);
   for (i = 0; i < n; i++){
      if(parr[i] == num)
         sfreq++;
   }
   MPI_Reduce(&sfreq, &tfreq, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   if(my_rank == 0)
      printf("The frequency of the number %d in the list is %d\n", num, tfreq);
   MPI_Finalize();
   free(parr);
   return 0;
}

