#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

/*void UpperCase(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}*/

void UpperCase(char str[]){
	for(int i=0; str[i]; i++){
		if(str[i] >= 'a' && str[i] <= 'z')
			str[i] = str[i] - ('a' - 'A');
	}
}

int isPalindrome(char str[]) {
    int left = 0;
    int right = strlen(str) - 1;

    while (left < right) {
        if (str[left] != str[right])
            return 0;
        left++;
        right--;
    }
    return 1;
}

int main() {
    int rank, size;
    char message[MAX];
    MPI_Status status;

    char pmessage[7][20] = {"Hi","hello","welcome","madam","malayalam","thank","program"};

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank != 0) {
        if (rank % 2 == 1) {                
            MPI_Send(pmessage[rank-1], strlen(pmessage[rank-1]) + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        } else {
            //sprintf(message, "madam");
	    //message[rank-1]
            MPI_Send(pmessage[rank-1], strlen(pmessage[rank-1]) + 1, MPI_CHAR, 0, 2, MPI_COMM_WORLD);
        }
    }
    
    else {
        for (int i = 1; i < size; i++) {
            MPI_Recv(message, MAX, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            int sender = status.MPI_SOURCE;
            int tag = status.MPI_TAG;
	    char recv[MAX];

            if (tag == 1) {
                UpperCase(message);
		printf("\n Uppercase conversion\nProcess %d(Type1):\n%s\n", sender,message);
                //printf("From process %d (Type 1): Uppercase string = %s\n", sender, message);
            } else if (tag == 2) {
		printf("\n Palindrome checking\n");
                if (isPalindrome(message))
                    printf("Process %d (Type 2)\n%s is a Palindrome\n", sender, message);
                else
                    printf("Process %d (Type 2)\n%s is not a Palindrome\n", sender, message);
            }
        }
    }

    MPI_Finalize();
    return 0;
}
