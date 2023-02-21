#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define MATRIX_SIZE 10000  
#define BLOCK_SIZE 100  

int main(int argc, char** argv) {
   
    int rank, size; 
   
    int i, j, number, count, global_count = 0; 
    int matrix_block[BLOCK_SIZE][MATRIX_SIZE]; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    if (rank == 0) {
        while (getchar() != '\n');  
        printf("Ingrese un numero: ");
        fflush(stdout);
        scanf("%d", &number); 
    }
    MPI_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_WORLD); 

    
    for (i = rank * BLOCK_SIZE; i < (rank + 1) * BLOCK_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            matrix_block[i - rank * BLOCK_SIZE][j] = rand() % MATRIX_SIZE;
        }
    }
  
    count = 0;
    for (i = 0; i < BLOCK_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            if (matrix_block[i][j] == number) {
                count++;
            }
        }
    }

    MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   
    if (rank == 0) {
        printf("El numero %d aparece %d veces en la matriz.\n", number, global_count);
    }
    MPI_Finalize();
    return 0;
}
