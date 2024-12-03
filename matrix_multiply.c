// Mehmet Efe Ergin 220201024
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MATRIX_SIZE 100

// Args to use at threads
typedef struct {
    int** matrix_a;
    int** matrix_b;
    int** result_matrix;
    int row;
    int col;
} ThreadData;

// Matrix creator function (if matrix going to be empty fill it with 0, else fill it randomly)
int** matrix_creator3000(int matrix_size, int isMatrixEmpty) {
    int** matrix = malloc(matrix_size * sizeof(int*));
    for (int i = 0; i < matrix_size; i++) {
        matrix[i] = malloc(matrix_size * sizeof(int));
        for (int j = 0; j < matrix_size; j++) {
            matrix[i][j] = (isMatrixEmpty == 0) ? (rand() % 100 + 1) : 0;
        }
    }
    return matrix;
}

// matrix multiply func, calling from threads
void* matrix_multiply(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    // multiply opearation 
    data->result_matrix[data->row][data->col] = 0;
    for (int k = 0; k < MATRIX_SIZE; k++) {
        data->result_matrix[data->row][data->col] += data->matrix_a[data->row][k] * data->matrix_b[k][data->col];
    }

    pthread_exit(NULL);
}

// creating threads and directing them to multiply func
void create_threads(int** matrix_a, int** matrix_b, int** result_matrix) {
    pthread_t threads[MATRIX_SIZE * MATRIX_SIZE];
    ThreadData thread_data[MATRIX_SIZE * MATRIX_SIZE];
    int thread_index = 0;

    // creating threads for every matrix element
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            thread_data[thread_index].matrix_a = matrix_a;
            thread_data[thread_index].matrix_b = matrix_b;
            thread_data[thread_index].result_matrix = result_matrix;
            thread_data[thread_index].row = i;
            thread_data[thread_index].col = j;

            pthread_create(&threads[thread_index], NULL, matrix_multiply, &thread_data[thread_index]);
            thread_index++;
        }
    }

    //thread joining
    for (int i = 0; i < thread_index; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    srand(time(NULL));

    // creating matrix using function
    int** matrix_a = matrix_creator3000(MATRIX_SIZE, 0);
    int** matrix_b = matrix_creator3000(MATRIX_SIZE, 0);
    int** result_matrix = matrix_creator3000(MATRIX_SIZE, 1);

    // print out matrices (first 2 matrices that are going to be used in multiply) 
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrix_a[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrix_b[i][j]);
        }
        printf("\n");
    }

     // start time
    clock_t start_time = clock();

    // calling create threads and multiply functions
    create_threads(matrix_a, matrix_b, result_matrix);

    // end time and calculate total time
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time ) / CLOCKS_PER_SEC * 1000;

    
    

    // print out final matrix
    printf("Result Matrix:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", result_matrix[i][j]);
        }
        printf("\n");
    }
    //print out total time elapsed
    printf("\nTotal time of execution: %.2f\n", elapsed_time);

    // clean memory
    for (int i = 0; i < MATRIX_SIZE; i++) {
        free(matrix_a[i]);
        free(matrix_b[i]);
        free(result_matrix[i]);
    }
    free(matrix_a);
    free(matrix_b);
    free(result_matrix);

    return 0;
}
