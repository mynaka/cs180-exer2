#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include<pthread.h>

//Data structure for matrix cell
typedef struct matrix{
	int row;
	int column;
	int element;
}MATRIX;

int N;
MATRIX **matA, **matB, **matC, **BTransposed;

//get values in the row through a thread
void* getRow(void* args){
    MATRIX* temp;
	temp = ((MATRIX**) args)[0];
	for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
		    (temp[i]).element += (matA[temp[i].row][j].element * BTransposed[i][temp[j].column].element);
        }
	}
    
	pthread_exit(NULL);
}

//Multiply matrix A and matrix B, assign result to matrix C
void threaded_rows_cached(MATRIX** A, MATRIX** B, MATRIX** C){
    pthread_t* tid = (pthread_t*)malloc(N * sizeof(pthread_t));
    BTransposed = (MATRIX**)malloc(N * sizeof(MATRIX*));
    for(int i = 0; i < N; i++){
        BTransposed[i] = (MATRIX*)malloc(N * sizeof(MATRIX));
        for (int j = 0; j < N; j++){
            BTransposed[i][j].row = i;
            BTransposed[i][j].column = j;
            BTransposed[i][j].element = B[j][i].element;            
        }
    }

    for(int i = 0; i < N; i++){
		pthread_create(&tid[i], NULL, getRow, (void *)&C[i]);
	}
	//join the threads
	for(int i = 0; i < N; i++){
		//joins the threads so that the thread will wait for the others
		pthread_join(tid[i], NULL);
	}
    free(tid);  //free pthreads
}

int main(){
    srand(time(0));

    printf("Input matrix size: ");
    scanf("%d", &N);                                        //input matrix size

    matA = (MATRIX**)malloc(N * sizeof(MATRIX*));
    matB = (MATRIX**)malloc(N * sizeof(MATRIX*));
    matC = (MATRIX**)malloc(N * sizeof(MATRIX*));

    for(int i = 0; i < N; i++){
        matA[i] = (MATRIX*)malloc(N * sizeof(MATRIX));            //ArrA and ArrB are factors with values ranging from -5 to 5
        matB[i] = (MATRIX*)malloc(N * sizeof(MATRIX));            //ArrC is the resultant that is initially a null matrix
        matC[i] = (MATRIX*)malloc(N * sizeof(MATRIX));
        for (int j = 0; j < N; j++){
            matA[i][j].row = i;
            matA[i][j].column = j;
            matA[i][j].element = (rand()%10)-5; 
            
            matB[i][j].row = i;
            matB[i][j].column = j;
            matB[i][j].element = (rand()%10)-5;

            matC[i][j].row = i;
            matC[i][j].column = j;
            matC[i][j].element = 0;
        }
    }

    //set up timer and run function
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    threaded_rows_cached(matA, matB, matC);
    gettimeofday(&stop, NULL);
	double t = (double)((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000000;
    /*
    printf("\n");
    for(int i = 0; i < N; i++){               //show results
        for (int j = 0; j < N; j++){
            printf("%3i ",matA[i][j].element);
        }
        printf("\n");
    }
    printf("\n");
    for(int i = 0; i < N; i++){               //show results
        for (int j = 0; j < N; j++){
            printf("%3i ",matB[i][j].element);
        }
        printf("\n");
    }
    printf("\n");
    for(int i = 0; i < N; i++){               //show results
        for (int j = 0; j < N; j++){
            printf("%3i ",matC[i][j].element);
        }
        printf("\n");
    }
    */
    printf("Process took %f seconds\n",t);
    for(int i = 0; i < N; i++){
        free(matA[i]);
        free(matB[i]);
        free(matC[i]);
        free(BTransposed[i]);
    }
    free(matA);
    free(matB);
    free(matC);
    free(BTransposed);
    return 0;
}