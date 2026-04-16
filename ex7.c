#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_SUBMISSIONS 10
#define MAX_TOTAL 10

int pending_submissions = 0;
int total_submitted = 0;
int evaluated_count = 0;

pthread_mutex_t mutex;
pthread_cond_t cond_not_empty;
pthread_cond_t cond_not_full;
pthread_cond_t cond_evaluated;

void* submitter(void* arg) {
    while (1) {
        sleep(2);

        pthread_mutex_lock(&mutex);

        if (total_submitted >= MAX_TOTAL) {
            pthread_cond_broadcast(&cond_not_empty);
            pthread_mutex_unlock(&mutex);
            break;
        }

        while (pending_submissions >= MAX_SUBMISSIONS) {
            printf("Submitter waiting (queue full)\n");
            pthread_cond_wait(&cond_not_full, &mutex);
        }

        pending_submissions++;
        total_submitted++;
        printf("Submitted! Pending = %d | Total Submitted = %d\n",
               pending_submissions, total_submitted);

        pthread_cond_signal(&cond_not_empty);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void* evaluator(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        while (pending_submissions == 0 && total_submitted < MAX_TOTAL) {
            printf("Evaluator waiting (no submissions)\n");
            pthread_cond_wait(&cond_not_empty, &mutex);
        }

        if (pending_submissions == 0 && total_submitted >= MAX_TOTAL) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        pending_submissions--;
        printf("Evaluated! Pending = %d\n", pending_submissions);

        pthread_cond_signal(&cond_not_full);
        pthread_cond_signal(&cond_evaluated);

        pthread_mutex_unlock(&mutex);

        sleep(2);
    }

    pthread_exit(NULL);
}

void* monitor(void* arg) {
    while (1) {
        printf("[Monitor] Pending = %d | Evaluated = %d\n",
               pending_submissions, evaluated_count);

        if (evaluated_count >= MAX_TOTAL)
            break;

        sleep(1);
    }

    pthread_exit(NULL);
}

void* publisher(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        while (evaluated_count >= total_submitted &&
               total_submitted < MAX_TOTAL) {
            pthread_cond_wait(&cond_evaluated, &mutex);
        }

        if (evaluated_count >= MAX_TOTAL) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        evaluated_count++;
        printf("Result Published! Total evaluated = %d\n\n", evaluated_count);

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2, t3, t4;

    printf("\n 2.Pthread program for answer script evaluation and monitoring  \n");
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_not_empty, NULL);
    pthread_cond_init(&cond_not_full, NULL);
    pthread_cond_init(&cond_evaluated, NULL);

    pthread_create(&t1, NULL, submitter, NULL);
    pthread_create(&t2, NULL, evaluator, NULL);
    pthread_create(&t3, NULL, monitor, NULL);
    pthread_create(&t4, NULL, publisher, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_not_empty);
    pthread_cond_destroy(&cond_not_full);
    pthread_cond_destroy(&cond_evaluated);

    printf("\nAll submissions processed successfully!\n");

    return 0;
}
