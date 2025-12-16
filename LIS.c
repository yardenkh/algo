#include <stdio.h>

int f(int A[],int i, int j){
    if (i < 0)
    {
        return 0;
    }
    if (A[j] < A[i])
    {
        return f(A, i - 1, j);
    }
    if (A[i] < A[j])
    {
        return 1 + f(A, i-1, i);
    }
}

int LIS(int A[], int n) {
    int maxLen = 0;
    for (int j = 0; j < n; j++) {
        int len = 1 + f(A, j - 1, j); // +1 for A[j] itself
        if (len > maxLen) {
            maxLen = len;
        }
    }
    return maxLen;
}


int main(int argc, char *argv[]) {
   int A[] = {10, 9, 2, 5, 3, 57, 101, 18};
    int n = sizeof(A) / sizeof(A[0]);
    printf("Length of LIS = %d\n", LIS(A, n));
    return 0;
}