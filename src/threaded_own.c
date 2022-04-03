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

//Data structure for storing arguments used in pthreads
typedef struct arguments{
    MATRIX** op1;       //operand 1
    MATRIX** op2;       //operand 2
    MATRIX** res;       //resultant
    int col;            //row and col to keep track of cells
    int row;
}ARGS;

int N, Dimensions;
MATRIX **matA, **matB, **matC;

//Add two matrices
MATRIX** add_matrix(MATRIX** A, MATRIX** B){
    int size = Dimensions/2;
    MATRIX** res = (MATRIX**)malloc(size * sizeof(MATRIX*));
    for(int i = 0; i < size; i++){
        res[i] = (MATRIX*)malloc(size * sizeof(MATRIX)); 
        for(int j = 0; j < size; j++){
            res[i][j].row = i;
            res[i][j].column = j;
            res[i][j].element = A[i][j].element + B[i][j].element;
        }
    }
    return res;
}

//Subtract two matrices
MATRIX** sub_matrix(MATRIX** A, MATRIX** B){
    int size = Dimensions/2;
    MATRIX** res = (MATRIX**)malloc(size * sizeof(MATRIX*));

    for(int i = 0; i < size; i++){
        res[i] = (MATRIX*)malloc(size * sizeof(MATRIX)); 
        for(int j = 0; j < size; j++){
            res[i][j].row = i;
            res[i][j].column = j;
            res[i][j].element = A[i][j].element - B[i][j].element;
        }
    }
    return res;
}

//get the value of a cell through matrix multiplication on a thread
void* getCell(void* args){
    ARGS* temp;
	temp = (ARGS*) args;
    int ans = 0;
    int size = Dimensions/2;
    int row = temp->res[temp->row][temp->col].row;
    int column = temp->res[temp->row][temp->col].column;
	for(int i = 0; i < size; i++){
		ans += (temp->op1[row][i].element * temp->op2[i][column].element);  //matrix multiplication
	}
    temp->res[row][column].element = ans;
	pthread_exit(NULL);
}

//Multiply matrix A and matrix B by threaded strassen's algorithm(modified), assign result to matrix C
void threaded_own(MATRIX** A, MATRIX** B, MATRIX** C){
    int newSize = Dimensions/2;
    pthread_t** tid = (pthread_t**)malloc(7 * sizeof(pthread_t*));
    for(int i = 0; i < 7; i++){
        tid[i] = (pthread_t*)malloc(newSize * newSize * sizeof(pthread_t));
    }

    MATRIX** A0 = (MATRIX**)malloc(newSize * sizeof(MATRIX*));
    MATRIX** A1 = (MATRIX**)malloc(newSize * sizeof(MATRIX*));
    MATRIX** A2 = (MATRIX**)malloc(newSize * sizeof(MATRIX*));
    MATRIX** A3 = (MATRIX**)malloc(newSize * sizeof(MATRIX*));
    MATRIX** B0 = (MATRIX**)malloc(newSize * sizeof(MATRIX*));
    MATRIX** B1 = (MATRIX**)malloc(newSize * sizeof(MATRIX*));
    MATRIX** B2 = (MATRIX**)malloc(newSize * sizeof(MATRIX*));
    MATRIX** B3 = (MATRIX**)malloc(newSize * sizeof(MATRIX*));
    for(int i = 0; i < newSize; i++){
        A0[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
        A1[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
        A2[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
        A3[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
        B0[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
        B1[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
        B2[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
        B3[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
    }

    for(int i = 0; i < newSize; i++){                            //split matrices into quadrants
        for(int j = 0; j < newSize; j++){                        //each of size/2 by size/2
            A0[i][j] = A[i][j];
            A1[i][j] = A[i][(newSize) + j];
            A2[i][j] = A[(newSize) + i][j];
            A3[i][j] = A[(newSize) + i][(newSize) + j];
            B0[i][j] = B[i][j];
            B1[i][j] = B[i][(newSize) + j];
            B2[i][j] = B[(newSize) + i][j];
            B3[i][j] = B[(newSize) + i][(newSize) + j];
        }
    }

    ARGS M0;
    M0.op1 = add_matrix(A0, A3);
    M0.op2 = add_matrix(B0, B3);
    M0.res = (MATRIX**)malloc(newSize * sizeof(MATRIX*));       //Res is an entirely new matrix so it must be allocated
    for(int i = 0; i < newSize; i++){
        M0.res[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
        for(int j = 0; j < newSize; j++){
            M0.res[i][j].row = i;                               //Assign params for res
            M0.res[i][j].column = j;
            M0.res[i][j].element = 0;

            M0.row = i;
            M0.col = j;
			pthread_create(&tid[0][(i*newSize)+j], NULL, getCell, (void *)&M0); //create thread
		}
    }

    ARGS M1;
    M1.op1 = add_matrix(A2, A3);
    M1.op2 = B0;
    M1.res = (MATRIX**)malloc(newSize * sizeof(MATRIX*));
    for(int i = 0; i < newSize; i++){
        M1.res[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
        for(int j = 0; j < newSize; j++){
            M1.res[i][j].row = i;
            M1.res[i][j].column = j;
            M1.res[i][j].element = 0;

            M1.row = i;
            M1.col = j;
			pthread_create(&tid[1][(i*newSize)+j], NULL, getCell, (void *)&M1);
		}
    }

    ARGS M2;
    M2.op1 = A0;
    M2.op2 = sub_matrix(B1, B3);
    M2.res = (MATRIX**)malloc(newSize * sizeof(MATRIX*));
    for(int i = 0; i < newSize; i++){
        M2.res[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
        for(int j = 0; j < newSize; j++){
            M2.res[i][j].row = i;
            M2.res[i][j].column = j;
            M2.res[i][j].element = 0;

            M2.row = i;
            M2.col = j;
			pthread_create(&tid[2][(i*newSize)+j], NULL, getCell, (void *)&M2);
		}
    }

    ARGS M3;
    M3.op1 = A3;
    M3.op2 = sub_matrix(B2, B0);
    M3.res = (MATRIX**)malloc(newSize * sizeof(MATRIX*));
    for(int i = 0; i < newSize; i++){
        M3.res[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
        for(int j = 0; j < newSize; j++){
            M3.res[i][j].row = i;
            M3.res[i][j].column = j;
            M3.res[i][j].element = 0;

            M3.row = i;
            M3.col = j;
			pthread_create(&tid[3][(i*newSize)+j], NULL, getCell, (void *)&M3);
		}
    }

    ARGS M4;
    M4.op1 = add_matrix(A0, A1);
    M4.op2 = B3;
    M4.res = (MATRIX**)malloc(newSize * sizeof(MATRIX*));
    for(int i = 0; i < newSize; i++){
        M4.res[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
		for(int j = 0; j < newSize; j++){
            M4.res[i][j].row = i;
            M4.res[i][j].column = j;
            M4.res[i][j].element = 0;

            M4.row = i;
            M4.col = j;
			pthread_create(&tid[4][(i*newSize)+j], NULL, getCell, (void *)&M4);
		}
	}

    ARGS M5;
    M5.op1 = sub_matrix(A2, A0);
    M5.op2 = add_matrix(B0, B1);
    M5.res = (MATRIX**)malloc(newSize * sizeof(MATRIX*));
    for(int i = 0; i < newSize; i++){
        M5.res[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
		for(int j = 0; j < newSize; j++){
            M5.res[i][j].row = i;
            M5.res[i][j].column = j;
            M5.res[i][j].element = 0;

            M5.row = i;
            M5.col = j;
			pthread_create(&tid[5][(i*newSize)+j], NULL, getCell, (void *)&M5);
		}
	}

    ARGS M6;
    M6.op1 = sub_matrix(A1, A3);
    M6.op2 = add_matrix(B2, B3);
    M6.res = (MATRIX**)malloc(newSize * sizeof(MATRIX*));
    for(int i = 0; i < newSize; i++){
        M6.res[i] = (MATRIX*)malloc(newSize * sizeof(MATRIX));
		for(int j = 0; j < newSize; j++){
            M6.res[i][j].row = i;
            M6.res[i][j].column = j;
            M6.res[i][j].element = 0;

            M6.row = i;
            M6.col = j;
			pthread_create(&tid[6][(i*newSize)+j], NULL, getCell, (void *)&M6);
		}
	}

    //join the threads
	for(int i=0; i<7; i++){
        for(int j = 0; j < newSize * newSize; j++){
            //joins the threads so that the thread will wait for the others
            pthread_join(tid[i][j], NULL);
        }
	}

    MATRIX** Q0 = sub_matrix(add_matrix(M0.res, M3.res), add_matrix(M4.res, M6.res));
    MATRIX** Q1 = add_matrix(M2.res, M4.res);
    MATRIX** Q2 = add_matrix(M1.res, M3.res);
    MATRIX** Q3 = add_matrix(sub_matrix(M0.res, M1.res), add_matrix(M2.res, M6.res));
    for(int i = 0; i < newSize; i++){                            //split matrices into quadrants
        for(int j = 0; j < newSize; j++){                        //each of size/2 by size/2
            C[i][j] = Q0[i][j];
            C[i][newSize + j] = Q1[i][j];
            C[newSize + i][j] = Q2[i][j];
            C[newSize + i][newSize + j] = Q3[i][j];
        }
    }

    for(int i = 0; i < newSize; i++){
        free(A0[i]);
        free(A1[i]);
        free(A2[i]);
        free(A3[i]);
        free(B0[i]);
        free(B1[i]);
        free(B2[i]);
        free(B3[i]);

        free(M0.res[i]);
        free(M1.res[i]);
        free(M2.res[i]);
        free(M3.res[i]);
        free(M4.res[i]);
        free(M5.res[i]);
        free(M6.res[i]);
    }
    free(A0);
    free(A1);
    free(A2);
    free(A3);
    free(B0);
    free(B1);
    free(B2);
    free(B3);

    free(M0.res);
    free(M1.res);
    free(M2.res);
    free(M3.res);
    free(M4.res);
    free(M5.res);
    free(M6.res);

    for(int i=0; i<7; i++){
        free(tid[i]);
	}
    free(tid);
}

int main(){
    srand(time(0));

    printf("Input matrix size: ");
    scanf("%d", &N);                                        //input matrix size

    if(N%2) Dimensions = N+1;                               //The algorithm only works if N is an even number
    else Dimensions = N;                                    //If N is odd, add 1 to the dimensions and assign 0 to extra rows and cols

    matA = (MATRIX**)malloc(Dimensions * sizeof(MATRIX*));
    matB = (MATRIX**)malloc(Dimensions * sizeof(MATRIX*));
    matC = (MATRIX**)malloc(Dimensions * sizeof(MATRIX*));

    for(int i = 0; i < Dimensions; i++){
        matA[i] = (MATRIX*)malloc(Dimensions * sizeof(MATRIX));            //ArrA and ArrB are factors with values ranging from -5 to 5
        matB[i] = (MATRIX*)malloc(Dimensions * sizeof(MATRIX));            //ArrC is the resultant that is initially a null matrix
        matC[i] = (MATRIX*)malloc(Dimensions * sizeof(MATRIX));
        for (int j = 0; j < Dimensions; j++){
            matA[i][j].row = i;
            matA[i][j].column = j;
            
            matB[i][j].row = i;
            matB[i][j].column = j;
            
            if((i >= N)||(j >= N)){
                matA[i][j].element = 0;                         //assign 0 to extra rows and columns
                matB[i][j].element = 0;
            }else{
                matA[i][j].element = (rand()%10)-5;
                matB[i][j].element = (rand()%10)-5;
            }

            matC[i][j].row = i;
            matC[i][j].column = j;
            matC[i][j].element = 0;
        }
    }

    //set up timer and run function
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    threaded_own(matA, matB, matC);
    gettimeofday(&stop, NULL);
	double t = (double)((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000000;

    printf("Process took %f seconds\n",t);
    for(int i = 0; i < Dimensions; i++){
        free(matA[i]);
        free(matB[i]);
        free(matC[i]);
    }
    free(matA);
    free(matB);
    free(matC);
    return 0;
}