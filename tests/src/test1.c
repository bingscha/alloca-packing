// C program for implementation of Bubble sort 
#include <stdio.h>
  
// Driver program to test above functions 
int main() 
{ 
    char a, b, c, d, e, f, g, h;
    a = 'a';
    b = 'b';
    c = 'c';
    d = 'd';
    e = 'e';
    f = 'f';
    g = 'g';
    h = 'h';
    int sum = 0;
    for (int k = 0;  k < 10; ++k) {

        if (k < 5) {
            sum += a + b + c + d;
        }
        else {
            sum += e + f + g + h;
        }
        printf("%d\n", sum);
    }
    
} 