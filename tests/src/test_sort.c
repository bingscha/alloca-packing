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

void merge_sort(int a[], int length) {
    merge_sort(a, 0, length-1);
}

void merge_sort(int a[], int low, int high) {
    if (low >= high)                  //Base case: 1 value to sort->sorted
      return;                         //(0 possible only on initial call)
    else {
      int mid = (low + high)/2;       //Approximate midpoint*
      merge_sort(a, low, mid);        //Sort low to mid part of array
      merge_sort(a, mid+1, high);     //Sort mid+1 to high part of array
      merge(a, low, mid, mid+1,high); //Merge sorted subparts of array
    }
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

void max_heapify(int a[], int i, int n)
{
  int l,r,largest,loc;
  l=2*i;
  r=(2*i+1);
  if((l<=n)&&a[l]>a[i])
    largest=l;
  else
    largest=i;
  if((r<=n)&&(a[r]>a[largest]))
    largest=r;
  if(largest!=i)
    {
      loc=a[i];
      a[i]=a[largest];
      a[largest]=loc;
      max_heapify(a, largest,n);
    }
}
void build_max_heap(int a[], int n)
{
  for(int k = n/2; k >= 1; k--)
    {
      max_heapify(a, k, n);
    }
}
void heap_sort(int a[], int n)
{

  build_max_heap(a,n);
  int i, temp;
  for (i = n; i >= 2; i--)
    {
      temp = a[i];
      a[i] = a[1];
      a[1] = temp;
      max_heapify(a, 1, i - 1);
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
    // todo
}