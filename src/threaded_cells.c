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
MATRIX **matA, **matB, **matC;

//get the value of a cell through a thread
void* getCell(void* args){
    MATRIX* temp;
	temp = (MATRIX*) args;
	temp->element = 0;
	for(int i = 0; i < N; i++){
		temp->element += (matA[temp->row][i].element * matB[i][temp->column].element);  //naive matrix multiplication
	}

	pthread_exit(NULL);
}

//Multiply matrix A and matrix B, assign result to matrix C
void threaded_cells(MATRIX** A, MATRIX** B, MATRIX** C){
    pthread_t* tid = (pthread_t*)malloc(N * N * sizeof(pthread_t));

    for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			pthread_create(&tid[(i*N)+j], NULL, getCell, (void *)&C[i][j]);
		}
	}
	//join the threads
	for(int i=0; i<N*N; i++){
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
    threaded_cells(matA, matB, matC);
    gettimeofday(&stop, NULL);
	double t = (double)((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000000;
    /*
    for(int i = 0; i < N; i++){               //show results
        for (int j = 0; j < N; j++){
            printf("%3i ",matC[i][j]->element);
        }
        printf("\n");
    }
    */
    printf("Process took %f seconds\n",t);
    for(int i = 0; i < N; i++){
        free(matA[i]);
        free(matB[i]);
        free(matC[i]);
    }
    free(matA);
    free(matB);
    free(matC);
    return 0;
}