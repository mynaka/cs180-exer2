#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
int N;

//Multiply matrix A and matrix B, assign result to matrix C
void mat_multiply(int** A, int** B, int** C){
    int res;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            res = 0;
            for(int k = 0; k < N; k++){
                res += A[i][k]*B[k][j];
            }
            C[i][j] = res;
        }
    }
}

int main(){
    srand(time(0));

    printf("Input matrix size: ");
    scanf("%d", &N);                                        //input matrix size

    int** arrA = (int**)malloc(N * sizeof(int*));
    int** arrB = (int**)malloc(N * sizeof(int*));
    int** arrC = (int**)malloc(N * sizeof(int*));

    for(int i = 0; i < N; i++){
        arrA[i] = (int*)malloc(N * sizeof(int));            //ArrA and ArrB are factors with values ranging from -5 to 5
        arrB[i] = (int*)malloc(N * sizeof(int));            //ArrC is the resultant that is initially a null matrix
        arrC[i] = (int*)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++){
            arrA[i][j] = (rand()%10)-5;            
            arrB[i][j] = (rand()%10)-5;
            arrC[i][j] = 0;
        }
    }

    //set up timer and run function
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    mat_multiply(arrA, arrB, arrC);
    gettimeofday(&stop, NULL);
	double t = (double)((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000000;
    /*
    for(int i = 0; i < N; i++){               //show results
        for (int j = 0; j < N; j++){
            printf("%3i ",arrC[i][j]);
        }
        printf("\n");
    }
    */
    printf("Process took %f seconds\n",t);
    for(int i = 0; i < N; i++){
        free(arrA[i]);
        free(arrB[i]);
        free(arrC[i]);
    }
    free(arrA);
    free(arrB);
    free(arrC);
    return 0;
}