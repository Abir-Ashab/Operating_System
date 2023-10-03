#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define N 3
#define M 4
#define L 4
#define K 3

void print_Resulting_Matrix(int matrix[N][K]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < K; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void multiplyMatrices(int A[N][M], int B[M][K], int C[N][K], int startRow, int numRows) {
    for (int i = startRow; i < startRow + numRows; ++i) {
        for (int j = 0; j < K; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < M; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {

    int A[N][M];
    int B[L][K];
    int C[N][K];

    if(M != L) printf("Multiplication of these two matrics is not possible");

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            A[i][j] = rand() % 10;
        }
    }
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < K; ++j) {
            B[i][j] = rand() % 10;
        }
    }

    printf("Matrix A:\n");
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    printf("\nMatrix B:\n");
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < K; ++j) {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }
    int shm_id_C = shmget(IPC_PRIVATE, N * K * sizeof(int), IPC_CREAT | 0666);
    int* sharedC = (int*)shmat(shm_id_C, NULL, 0);

    int numProcesses = N;

    for (int i = 0; i < numProcesses; ++i) {
        pid_t childPID = fork();
        if (childPID == 0) {
            int startRow = i;
            int numRows = 1;
            multiplyMatrices(A, B, C, startRow, numRows);
            printf("\nResulting Matrix C:\n");
            print_Resulting_Matrix((int (*)[K]) sharedC);
            for (int r = 0; r < numRows; ++r) {
                for (int j = 0; j < K; ++j) {
                    sharedC[(startRow + r) * K + j] = C[startRow + r][j];
                }
            }
            exit(0);
        }
    }

    for (int i = 0; i < numProcesses; ++i) {
        wait(NULL);
    }
    printf("\nResulting Matrix C:\n");
    print_Resulting_Matrix((int (*)[K]) sharedC);
    shmdt(sharedC);
    shmctl(shm_id_C, IPC_RMID, NULL);

    return 0;
}