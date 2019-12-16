#include <stdlib.h>
#include <stdio.h>

#define SIZE 100000
int arr[SIZE];

int main() {
    srand(0);
    
    for (int i = 0; i < SIZE; ++i) {
        arr[i] = (int) (rand() * SIZE);
    }
    int i, j, tmp;
    for (i = 1; i < SIZE - 1; i++) {
        j = i;
        while (j > 0 && arr[j - 1] > arr[j]) {
                tmp = arr[j];
                arr[j] = arr[j - 1];
                arr[j - 1] = tmp;
                j--;
        }
    }
    for (int i = 0; i < SIZE; ++i) {
      printf("%d ", arr[i]);
    }
    printf("\n");
}