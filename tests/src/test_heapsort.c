#include <stdlib.h>
#include <stdio.h>

#define SIZE 10000000
int arr[SIZE];

void heapify(int *unsorted, int index, int heap_size) {
    int temp;
    int largest = index;
    int left_index = 2 * index;
    int right_index = 2 * index + 1;
    if (left_index < heap_size && *(unsorted + left_index) > *(unsorted + largest)) {
        largest = left_index;
    }
    if (right_index < heap_size && *(unsorted + right_index) > *(unsorted + largest)) {
        largest = right_index;
    }

    if (largest != index) {
        temp = *(unsorted + largest);
        *(unsorted + largest) = *(unsorted + index);
        *(unsorted + index) = temp;
        heapify(unsorted, largest, heap_size);
    }
}

void heap_sort(int *unsorted, int n) {
    int temp;
    for (int i = n / 2 - 1; i > -1; i--) {
        heapify(unsorted, i, n);
    }
    for (int i = n - 1; i > 0; i--) {
        temp = *(unsorted);
        *(unsorted) = *(unsorted + i);
        *(unsorted + i) = temp;
        heapify(unsorted, 0, i);
    }
}

int main() {
    srand(0);

    for (int i = 0; i < SIZE; ++i) {
        arr[i] = (int) (rand() * SIZE);
    }
    heap_sort(arr, SIZE - 1);
    for (int i = 0; i < SIZE; ++i) {
      printf("%d ", arr[i]);
    }
    printf("\n");
}