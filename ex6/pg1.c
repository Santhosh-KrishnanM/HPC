#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int a, b;
    char op; // '+', '-', '*', '/'
} Job;

Job work_buffer;
sem_t sem_empty, sem_full, sem_mutex;
FILE *result_file;
int jobs_to_do = 8;

void* compute_thread(void* arg) {
    char target_op = *(char*)arg;
    pthread_t tid = pthread_self();
    while (1) {
        sem_wait(&sem_full); 
        sem_wait(&sem_mutex);
        if (work_buffer.op == target_op) {
            int res = 0;
            float res_div = 0;
            if (target_op == '+') res = work_buffer.a + work_buffer.b;
            else if (target_op == '-') res = work_buffer.a - work_buffer.b;
            else if (target_op == '*') res = work_buffer.a * work_buffer.b;
            else if (target_op == '/') {
                if (work_buffer.b != 0) res_div = (float)work_buffer.a / work_buffer.b;
            }
            result_file = fopen("result_file.txt", "a");
            if (target_op == '/')
                fprintf(result_file, "Thread - %d: %d / %d = %.2f\n", int(tid), work_buffer.a, work_buffer.b, res_div);
            else
                fprintf(result_file, "Thread - %d: %d %c %d = %d\n", int(tid),  work_buffer.a, target_op, work_buffer.b, res);
            fclose(result_file);
            printf("  Thread [%d] performed %c operation\n", int(tid), target_op);         
            sem_post(&sem_mutex);
            sem_post(&sem_empty); 
        } 
	else {
            sem_post(&sem_mutex);
            sem_post(&sem_full);
            usleep(1000);
        }   
        if (jobs_to_do <= 0) break;
    }
    return NULL;
}

int main() {
    pthread_t threads[4];
    char ops[4] = {'+', '-', '*', '/'};
    printf("\n  Pthreads synchronization using Semaphores\n");
    srand(time(NULL));
    result_file = fopen("result_file.txt", "w");
    fclose(result_file);
    sem_init(&sem_empty, 0, 1); 
    sem_init(&sem_full, 0, 0);  
    sem_init(&sem_mutex, 0, 1);
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, compute_thread, &ops[i]);
    }
    for (int i = 0; i < 8; i++) {
        sem_wait(&sem_empty); 
        sem_wait(&sem_mutex); 
        work_buffer.a = rand() % 50 + 1;
        work_buffer.b = rand() % 10 + 1;
        work_buffer.op = ops[rand() % 4];
        printf("Evaluating the expression: %d %c %d\n", work_buffer.a, work_buffer.op, work_buffer.b);
        sem_post(&sem_mutex);
        sem_post(&sem_full);        
        jobs_to_do--;
        sleep(1);
    }
    printf("\nResults are stored in result_file.txt file.\n\n");
    return 0;
}
