#include <stdlib.h>
#include <stdio.h>

#define N 750
  
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
  
    int x, y, z; 
    for (x = 0; x < N; x++) 
    { 
        for (y = 0; y < N; y++) 
        { 
            res[x][y] = 0; 
            for (z = 0; z < N; z++) 
                res[x][y] += mat1[x][z] *  
                             mat2[z][y]; 
        } 
    } 

    for (x = 0; x < N; x++) 
    { 
        for (y = 0; y < N; y++) 
        printf("%d", res[x][y]);
        printf("\n");
    } 
  
    return 0; 
} 