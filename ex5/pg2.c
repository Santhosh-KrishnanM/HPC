#include <stdio.h>
#include <pthread.h>
#define MAX 10

int A[MAX][MAX], B[MAX][MAX];
int add[MAX][MAX], mul[MAX][MAX];
int n;

void* mat_add(void* arg) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            add[i][j] = A[i][j] + B[i][j];
        }
    }
    /*printf("\nResult:\n");
    for (i = 0; i < n; i++) {
       for (j = 0; j < n; j++)
	  printf("%d ", add[i][j]);
       printf("\n");
    }*/

    pthread_exit(NULL);
}


void* mat_mul(void* arg) {
    int i, j, k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            mul[i][j] = 0;
            for (k = 0; k < n; k++) {
                mul[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    /*printf("\nResult:\n");
    for (i = 0; i < n; i++) {
       for (j = 0; j < n; j++)
	  printf("%d ", mul[i][j]);
       printf("\n");
    }*/

    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;
    int i, j;
    printf("\n2.Pthread program to perform matrix addition and matrix multiplication using 2 threads\n\n");

    printf("Enter matrix size (N): ");
    scanf("%d", &n);

    printf("\nEnter Matrix A:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    printf("\nEnter Matrix B:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            scanf("%d", &B[i][j]);

    pthread_create(&t1, NULL, mat_add, NULL);
    pthread_create(&t2, NULL, mat_mul, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nResult: (Addition)\n");
    for (i = 0; i < n; i++) {
       for (j = 0; j < n; j++)
	  printf("%d ", add[i][j]);
       printf("\n");
    } 

    printf("\nResult: (Multiplication)\n");
    for (i = 0; i < n; i++) {
       for (j = 0; j < n; j++)
	  printf("%d ", mul[i][j]);
       printf("\n");
    }

    return 0;
}
