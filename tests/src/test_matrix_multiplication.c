#include <stdlib.h>
#include <stdio.h>

#define N 500
  
// This function multiplies  
// mat1[][] and mat2[][], and  
// stores the result in res[][] 
void multiply(int mat1[][N], 
              int mat2[][N],  
              int res[][N]) 
{ 
    int i, j, k; 
    for (i = 0; i < N; i++) 
    { 
        for (j = 0; j < N; j++) 
        { 
            res[i][j] = 0; 
            for (k = 0; k < N; k++) 
                res[i][j] += mat1[i][k] *  
                             mat2[k][j]; 
        } 
    } 
} 
  
// Driver Code 
int main() 
{ 
    int i, j; 
    int res[N][N]; // To store result
    int mat1[N][N];
    int mat2[N][N];
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            res[i][j] = 0;
            mat1[i][j] = rand() * N;
            mat2[i][j] = rand() * N;
        }
    }  
  
    multiply(mat1, mat2, res); 
    for (i = 0; i < N; i++) 
    { 
        for (j = 0; j < N; j++) 
        printf("%d", res[i][j]);
        printf("\n");
    } 
  
    return 0; 
} 