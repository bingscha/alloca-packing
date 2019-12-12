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

void merge(int a[], int left_low, int left_high, int right_low, int right_high) 
{ 
    int length = right_high-left_low+1;
    int temp[length];
    int left = left_low;
    int right = right_low;
    for (int i = 0; i < length; ++i) { 
        if (left > left_high)
            temp[i] = a[right++];
        else if (right > right_high)
            temp[i] = a[left++];
        else if (a[left] <= a[right])
            temp[i] = a[left++];
        else
            temp[i] = a[right++]; 
    }
    
    for (int i=0; i< length; ++i) 
        a[left_low++] = temp[i];
}

void merge_sort_helper(int a[], int low, int high) {
    if (low >= high)                  //Base case: 1 value to sort->sorted
      return;                         //(0 possible only on initial call)
    else {
      int mid = (low + high)/2;       //Approximate midpoint*
      merge_sort_helper(a, low, mid);        //Sort low to mid part of array
      merge_sort_helper(a, mid+1, high);     //Sort mid+1 to high part of array
      merge(a, low, mid, mid+1,high); //Merge sorted subparts of array
    }
}

void merge_sort(int a[], int length) {
    merge_sort_helper(a, 0, length-1);
}

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

    quick_sort(arr, 0, SIZE - 1);
    for (int i = 0; i < SIZE; ++i) {
      printf("%d ", arr[i]);
    }
    printf("\n");


    for (int i = 0; i < SIZE; ++i) {
        arr[i] = (int) (rand() * SIZE);
    }

    merge_sort(arr, SIZE);
    for (int i = 0; i < SIZE; ++i) {
      printf("%d ", arr[i]);
    }
    printf("\n");
    
    for (int i = 0; i < SIZE; ++i) {
        arr[i] = (int) (rand() * SIZE);
    }
    insertion_sort(arr, SIZE - 1);
    for (int i = 0; i < SIZE; ++i) {
      printf("%d ", arr[i]);
    }
    printf("\n");
}