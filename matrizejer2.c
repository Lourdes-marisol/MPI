#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000

int main(int argc, char **argv)
{
    int rank, size;
    int i, j, k; 
    double sum[N]; 
    double max[N]; 
    double diag[N][N]; 
    double local_diag[N]; 
    double local_sum[N], local_max[N]; 
    double max_sum=0.0; 
    int max_sum_index=0; 
    int global_max_rank, global_sum_rank; 
    int local_start, local_end; 
    double diagonal_sum[N], diagonal_max[N];
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    local_start = rank * N / size; 
    local_end = (rank + 1) * N / size; 
    srand(time(NULL)); 
    for (k = 0; k < N; k++) 
    {
        for (i = local_start; i < local_end; i++) 
        {
            for (j = 0; j < N; j++) 
            {
                if (i + j == k) 
                {
                    local_diag[i - local_start] = rand() % 100; 
                    break; 
                }
            }
        }
        local_sum[k] = 0.0; 
        local_max[k] = local_diag[0]; 
        diagonal_sum[k]=0.0; 
        diagonal_max[k]=local_diag[0]; 
        for (i = 0; i < local_end - local_start; i++) 
        {
            local_sum[k] += local_diag[i]; 
            diagonal_sum[k] += local_diag[i]; 
            if (local_diag[i] > local_max[k]) 
            {
                local_max[k] = local_diag[i]; 
            }
            if (local_diag[i] > diagonal_max[k]) 
            {
                diagonal_max[k] = local_diag[i]; 
            }
        }
        MPI_Reduce(&local_sum[k], &sum[k], 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
        MPI_Reduce(&local_max[k], &max[k], 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 
        if (sum[k] > max_sum) { 
            max_sum = sum[k]; 
            max_sum_index = k; 
        }

    }
    if (rank == 0) 
    {
        double global_sum = 0.0; 
        double global_max = max[0]; 
        int global_max_index = 0;  
        int global_sum_index = 0; 
        for (k = 0; k < N; k++) 
        {
            global_sum += sum[k]; 
            if (max[k] > global_max) 
            {
                global_max = max[k]; 
                global_max_index = k; 
            }
            if (sum[k] > sum[global_sum_index])
            {
                global_sum_index = k;
            }
        }
        
        printf("Suma de cada diagonal:\n");
        for (k = 0; k < N; k++)
        {
            printf("Diagonal %d: %lf\n", k+1, diagonal_sum[k]);
        }
        printf("Valor máximo de cada diagonal:\n");
        for (k = 0; k < N; k++)
        {
            printf("Diagonal %d: %lf\n", k+1, diagonal_max[k]);
        }
        printf("Valor más alto de las diagonales: %lf\n", global_max);
        printf("La diagonal %d tiene la suma más grande: %lf\n", max_sum_index+1, sum[max_sum_index]);
        
    }
    MPI_Finalize(); 
    return 0;
}
