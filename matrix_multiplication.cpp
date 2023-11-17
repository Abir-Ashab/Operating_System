#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <bits/stdc++.h>

#define N 4
#define M 3
#define L 3
#define K 4

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
    int D[N][K];
    clock_t start_t = clock();
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < K; ++j) {
            D[i][j] = 0;
            for (int k = 0; k < M; ++k) {
                D[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    clock_t end_t = clock();
    double exec_t = (double) (end_t - start_t)/CLOCKS_PER_SEC;
    printf("\nMatrix A:\n");
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
    // Inter-Process Communication(IPC)
    // IPC_PRIVATE : might be used with shmget to create a new shared memory segment, not about making private,Its just an uses for uniqueness.There is nothing exist like IPC_PUBLIC
    // IPC_CREAT: Create the IPC resource if it doesn't exist.
    // shm_id_C : used to store the shared memory segment ID for matrix C
    // 0666(octal number) : It specifies read and write permissions for all users 
    // IPC_CREAT | 0666 :  It is a flag which ensures that the resource is created with adequate permissions for all users
    // sharedC : a pointer to the shared memory segment where matrix C will be stored.

    key_t key = IPC_PRIVATE;
    int shm_id_C = shmget(key, N * K * sizeof(int), IPC_CREAT | 0666);
    int* sharedC = (int*)shmat(shm_id_C, NULL, 0);
    clock_t start_time = clock();
    int numProcesses;
    int a = N, b = K;
    if(a < b) numProcesses = b;
    else numProcesses = a;

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
            // terminate process
            exit(0);
        }    
    }
    // used to ensure that the parent process waits for all of its child processes to complete before proceeding further.
    for (int i = 0; i < numProcesses; ++i) {
        wait(NULL);
    }
    clock_t end_time = clock();
    double exec_time = (double) (end_time - start_time)/CLOCKS_PER_SEC;
    printf("\nFinal Resulting Matrix C:\n");
    print_Resulting_Matrix((int (*)[K]) sharedC);
    printf("Execution time : %f\n", exec_time);
    double efficiency = (exec_t - exec_time) * 100;
    printf("Efficiency of the matrices is %f percent", efficiency);
    // Shared Memory Cleanup
    shmdt(sharedC);
    shmctl(shm_id_C, IPC_RMID, NULL);
    return 0;
}
