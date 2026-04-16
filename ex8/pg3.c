#include <stdio.h>
#include <omp.h>

int main() {
    int n, constant;

    printf("1. OpenMP program to perform the add of array elements with a constant\n");
    printf("Enter size of array: ");
    scanf("%d", &n);

    int A[n];

    printf("Enter elements of array:\n");
    for(int i = 0; i < n; i++) {
        scanf("%d", &A[i]);
    }

    printf("Enter constant value: ");
    scanf("%d", &constant);

    #pragma omp parallel for
    for(int i = 0; i < n; i++) {
        A[i] = A[i] + constant;
    }

    printf("Updated array:\n");
    for(int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }

    return 0;
}
