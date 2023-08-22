#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main() {
    fork();
    printf("Pid is %d", getpid());
    return 0;
}