#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int thread_count;
void *hello(void* rank){
   long my_rank = (long)rank;
   printf("Hello all. This is thread %ld\n", my_rank);
   //pthread_exit(NULL);
}

int main(int argc, char* argv[]){
   printf("\n1.Pthread program to print hello message\n\n");
   long thread;
   pthread_t* thread_handles;
   thread_count = strtol(argv[1], NULL, 10);
   thread_handles = (pthread_t*)malloc(thread_count*sizeof(pthread_t));
   for(thread = 0; thread < thread_count; thread++){
      //thread 
      pthread_create(&thread_handles[thread], NULL, hello, (void*)thread);
   }
   printf("Hello from the main thread\n");
   for(thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);
   free(thread_handles);
   return 0;
}
