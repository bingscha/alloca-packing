#include <stdlib.h>
#include <stdio.h>

#define SIZE 10000000
int arr[SIZE];

void quick_sort(int *array, int low, int high)
{
    int i = low;
    int j = high;
    int pivot = array[(i + j) / 2];
    int temp;

    while (i <= j)
    {
        while (array[i] < pivot)
            i++;
        while (array[j] > pivot)
            j--;
        if (i <= j)
        {
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
            j--;
        }
    }
    if (j > low)
        quick_sort(array, low, j);
    if (i < high)
        quick_sort(array, i, high);
}

int main() {
    srand(0);

    for (int i = 0; i < SIZE; ++i) {
        arr[i] = (int) (rand() * SIZE);
    }
    quick_sort(arr, SIZE - 1);
    for (int i = 0; i < SIZE; ++i) {
      printf("%d ", arr[i]);
    }
    printf("\n");
}