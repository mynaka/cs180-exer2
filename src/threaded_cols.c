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

//get values in the row through a thread
void* getCol(void* args){
    MATRIX* temp, *BCol;
	temp = ((MATRIX**) args)[0];
    BCol = (MATRIX*)malloc(N*sizeof(MATRIX));
    for(int j = 0; j < N; j++){                 //creates a Matrix array composed of the elements in the required column of matrixV
		BCol[j] = matB[j][temp[0].row];
    }
    
	for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
		    (temp[i]).element += (matA[temp[i].column][j].element * BCol[j].element);   //computes for elements within the column
        }
	}
    free(BCol);     //free function matrices

	pthread_exit(NULL);         //returns thread
}

//Multiply matrix A and matrix B, assign result to matrix C
void threaded_cols(MATRIX** A, MATRIX** B, MATRIX** C){
    pthread_t* tid = (pthread_t*)malloc(N * sizeof(pthread_t));

    for(int i = 0; i < N; i++){
		pthread_create(&tid[i], NULL, getCol, (void *)&C[i]);
	}
	//join the threads
	for(int i = 0; i < N; i++){
		//joins the threads so that the thread will wait for the others
		pthread_join(tid[i], NULL);
	}
    
    int** CTransposed = (int**)malloc(N * sizeof(int*));        //since the function returned the column results in rows,
    for(int i = 0; i < N; i++){                                 //matrixC will be transposed
        CTransposed[i] = (int*)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++){
            CTransposed[i][j] = C[j][i].element;            
        }
    }

    for(int i = 0; i < N; i++){                                 //then reassigned to matrixC
        for (int j = 0; j < N; j++){
            C[i][j].element = CTransposed[i][j];            
        }
    }

    for(int i = 0; i < N; i++){                                 //then reassigned to matrixC
        free(CTransposed[i]);            
    }
    free(CTransposed);      //free temporary C matrix
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
    threaded_cols(matA, matB, matC);
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
    }
    free(matA);
    free(matB);
    free(matC);
    return 0;
}