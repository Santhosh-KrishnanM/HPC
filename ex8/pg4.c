#include <stdio.h>
#include <omp.h>

int main() {
    int n;

    printf("2. OpenMP program to perform the sum of the 2 arrays\n");
    printf("Enter size of arrays: ");
    scanf("%d", &n);

    int A[n], B[n], C[n];

    printf("Enter elements of array A:\n");
    for(int i = 0; i < n; i++) {
        scanf("%d", &A[i]);
    }

    printf("Enter elements of array B:\n");
    for(int i = 0; i < n; i++) {
        scanf("%d", &B[i]);
    }

    #pragma omp parallel for
    for(int i = 0; i < n; i++) {
        C[i] = A[i] + B[i];
    }

    printf("Resultant array:\n");
    for(int i = 0; i < n; i++) {
        printf("%d ", C[i]);
    }

    return 0;
}
