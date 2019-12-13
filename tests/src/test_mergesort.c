#include <stdlib.h>
#include <stdio.h>

#define SIZE 5000000
int arr[SIZE];

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

int main() {
    srand(0);

    for (int i = 0; i < SIZE; ++i) {
        arr[i] = (int) (rand() * SIZE);
    }
    merge_sort(arr, SIZE - 1);
    for (int i = 0; i < SIZE; ++i) {
      printf("%d ", arr[i]);
    }
    printf("\n");
}