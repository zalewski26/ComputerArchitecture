#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void* getResult();
void printMatrix();

int size;
int numOfThreads;
int* matrixA;
int* matrixB;
int* matrix_result;
int current = 0;
pthread_mutex_t lock;

int main(int argc, char** argv){
	size = atoi(argv[1]);
	numOfThreads = atoi(argv[2]);
	matrixA = malloc(sizeof (int) * size * size);
	matrixB = malloc(sizeof (int) * size * size);
	matrix_result = malloc(sizeof (int) * size * size);
	pthread_mutex_init(&lock, NULL);
	pthread_t threads[numOfThreads];

	for (int i = 0; i < size * size; i++){
		matrixA[i] = rand() % 2;
		matrixB[i] = rand() % 2;
	}

	for (int i = 0; i < numOfThreads; i++){
		pthread_create(&threads[i], NULL, getResult, 0);
	}
	for (int i = 0; i < numOfThreads; i++) {
		pthread_join(threads[i], NULL);
		printf("thread %d finished\n", i);
	}
	pthread_mutex_destroy(&lock);
	printMatrix();
}

void* getResult(){
	pthread_mutex_lock(&lock);
	int local_current = current;
	if (local_current == size){
		pthread_mutex_unlock(&lock);
		pthread_exit(0);
	}
	current++;
	printf("thread %d starting row %d\n", (int) pthread_self(), local_current);
	pthread_mutex_unlock(&lock);

	int row = local_current;
	int result;

	for (int i = 0; i < size; i++){
		result = 0;
		for (int j = 0; j < size; j++){
			if ((matrixA[j + row * size] && matrixB[i + j * size])){
				result = 1;
				break;
			}
		}
		matrix_result[row * size + i] = result;
	}
	getResult();
}

void printMatrix(){
	printf("A:\n");
	for (int i = 0; i < size * size; i++){
		if (i > 0 && i%size == 0)
			printf("\n");
		printf("%d ", matrixA[i]);
	}
	printf("\n\nB:\n");
	for (int i = 0; i < size * size; i++){
		if (i > 0 && i%size == 0)
			printf("\n");
		printf("%d ", matrixB[i]);
	}
	printf("\n\nResult:\n");
	for (int i = 0; i < size * size; i++){
		if (i > 0 && i%size == 0)
			printf("\n");
		printf("%d ", matrix_result[i]);
	}
	printf("\n");
}