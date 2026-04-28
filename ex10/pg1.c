#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define QUEUE_SIZE 50
#define SEQ_LENGTH 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define TOTAL_SEQUENCES 10

typedef struct {
    int data[SEQ_LENGTH];
} Sequence;

Sequence queue[QUEUE_SIZE];
int front = 0, rear = 0, count = 0;

omp_lock_t queue_lock;

double global_mean_sum = 0.0;
double global_variance_sum = 0.0;
int processed_count = 0;

void enqueue(Sequence seq) {
    omp_set_lock(&queue_lock);

    if (count < QUEUE_SIZE) {
        queue[rear] = seq;
        rear = (rear + 1) % QUEUE_SIZE;
        count++;
    }

    omp_unset_lock(&queue_lock);
}

int dequeue(Sequence *seq) {
    int success = 0;

    omp_set_lock(&queue_lock);

    if (count > 0) {
        *seq = queue[front];
        front = (front + 1) % QUEUE_SIZE;
        count--;
        success = 1;
    }

    omp_unset_lock(&queue_lock);
    return success;
}

void producer(int id) {
    for (int i = 0; i < TOTAL_SEQUENCES / NUM_PRODUCERS; i++) {
        Sequence seq;
        for (int j = 0; j < SEQ_LENGTH; j++) {
            seq.data[j] = rand() % 100;
        }

        enqueue(seq);
        #pragma omp critical
        {
            printf("Producer %d produced: ", id);
            for (int j = 0; j < SEQ_LENGTH; j++)
                printf("%d ", seq.data[j]);
            printf("\n");
        }
    }
}

void consumer(int id) {
    while (1) {
        Sequence seq;

        if (dequeue(&seq)) {
            double sum = 0.0, mean, variance = 0.0;

            for (int i = 0; i < SEQ_LENGTH; i++)
                sum += seq.data[i];

            mean = sum / SEQ_LENGTH;
            for (int i = 0; i < SEQ_LENGTH; i++)
                variance += pow(seq.data[i] - mean, 2);

            variance /= SEQ_LENGTH;
            #pragma omp atomic
            global_mean_sum += mean;

            #pragma omp atomic
            global_variance_sum += variance;

            #pragma omp atomic
            processed_count++;
            #pragma omp critical
            {
                printf("Consumer %d consumed: ", id);
                for (int i = 0; i < SEQ_LENGTH; i++)
                    printf("%d ", seq.data[i]);

                printf("| Mean = %.2f, Variance = %.2f\n", mean, variance);
            }
        }
        else {
            if (processed_count >= TOTAL_SEQUENCES)
                break;
        }
    }
}

int main() {
   printf("  2. OpenMP program to compute the mean and variance for each sequence\n");
   omp_init_lock(&queue_lock);

    #pragma omp parallel num_threads(NUM_PRODUCERS + NUM_CONSUMERS)
    {
        int tid = omp_get_thread_num();

        if (tid < NUM_PRODUCERS)
            producer(tid);
        else
            consumer(tid - NUM_PRODUCERS);
    }

    omp_destroy_lock(&queue_lock);

    // Final results
    printf("\nFinal Global Statistics:\n");
    printf("Average Mean = %.2f\n", global_mean_sum / processed_count);
    printf("Average Variance = %.2f\n", global_variance_sum / processed_count);

    return 0;
}
