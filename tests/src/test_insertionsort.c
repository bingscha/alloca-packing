#include <stdlib.h>
#include <stdio.h>

#define SIZE 10000000
int arr[SIZE];

void insertion_sort(int arr[], int length) {
      int i, j, tmp;
      for (i = 1; i < length; i++) {
            j = i;
            while (j > 0 && arr[j - 1] > arr[j]) {
                  tmp = arr[j];
                  arr[j] = arr[j - 1];
                  arr[j - 1] = tmp;
                  j--;
            }
      }
}

int main() {
    srand(0);
    
    for (int i = 0; i < SIZE; ++i) {
        arr[i] = (int) (rand() * SIZE);
    }
    insertion_sort(arr, SIZE - 1);
    for (int i = 0; i < SIZE; ++i) {
      printf("%d ", arr[i]);
    }
    printf("\n");
}