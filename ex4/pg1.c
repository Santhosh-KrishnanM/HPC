#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    char roll[20];
    float total_mark;
    char grade;
} Student;

char find_grade(float marks) {
    if (marks > 90 && marks <= 100) return 'O';
    else if (marks > 80 && marks <= 90) return 'A';
    else if (marks > 70 && marks <= 80) return 'B';
    else if (marks > 60 && marks <= 70) return 'C';
    else return 'D';
}

int main(int argc, char *argv[]) {

    int rank, size, n = 0, i;
    MPI_Datatype MPI_STUDENT;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int blocklengths[4] = {50, 20, 1, 1};
    MPI_Aint displacements[4], base_address;
    MPI_Datatype types[4] = {MPI_CHAR, MPI_CHAR, MPI_FLOAT, MPI_CHAR};

    Student temp;

    MPI_Get_address(&temp, &base_address);
    MPI_Get_address(&temp.name, &displacements[0]);
    MPI_Get_address(&temp.roll, &displacements[1]);
    MPI_Get_address(&temp.total_mark, &displacements[2]);
    MPI_Get_address(&temp.grade, &displacements[3]);

    for (i = 0; i < 4; i++) {
        displacements[i] -= base_address;
    }

    MPI_Type_create_struct(4, blocklengths, displacements, types, &MPI_STUDENT);
    MPI_Type_commit(&MPI_STUDENT);

    Student *students = NULL;
    Student *local_students;
    int local_n;

    if (rank == 0) {

        FILE *fp = fopen("students.txt", "r");
        if (!fp) {
            printf("Error opening student.txt\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        students = malloc(100 * sizeof(Student));
        n = 0;

        while (fscanf(fp, "%s %s %f", students[n].name, students[n].roll, &students[n].total_mark) == 3) {
            n++;
        }

        fclose(fp);

        if (n % size != 0) {
            printf("Number of students not divisible by processes\n");
