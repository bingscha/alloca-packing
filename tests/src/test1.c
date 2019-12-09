// C program for implementation of Bubble sort 
#include <stdio.h>
  
// Driver program to test above functions 
int main() 
{ 
    unsigned char a, b, c, d, e, f, g, h;
    a = 0;
    b = 1;
    c = 2;
    d = 3;
    e = 4;
    f = 5;
    g = 6;
    h = 7;
    unsigned int sum = 0;
    for (unsigned int k = 0;  k < 100000000; ++k) {
        if (k % 2 == 0) {
            sum += (a + b + c + d + e + f + g + h);
            sum += a * b;
            sum += b * c;
            sum += c * d;
            sum += d * e;
            sum += e * f;
            sum += f * g;
            sum += g * h;


            sum += a * b;
            sum += b * c;
            sum += c * d;
            sum += d * e;
            sum += e * f;
            sum += f * g;
            sum += g * h;

            sum += a * b;
            sum += b * c;
            sum += c * d;
            sum += d * e;
            sum += e * f;
            sum += f * g;
            sum += g * h;

            sum += a * b;
            sum += b * c;
            sum += c * d;
            sum += d * e;
            sum += e * f;
            sum += f * g;
            sum += g * h;

            sum += a * b;
            sum += b * c;
            sum += c * d;
            sum += d * e;
            sum += e * f;
            sum += f * g;
            sum += g * h;

            sum += a * b;
            sum += b * c;
            sum += c * d;
            sum += d * e;
            sum += e * f;
            sum += f * g;
            sum += g * h;
        }
        else {
            sum -= (a + b + c + d + e + f + g + h);
            sum -= a * b;
            sum -= b * c;
            sum -= c * d;
            sum -= d * e;
            sum -= e * f;
            sum -= f * g;
            sum -= g * h;

            sum -= a * b;
            sum -= b * c;
            sum -= c * d;
            sum -= d * e;
            sum -= e * f;
            sum -= f * g;
            sum -= g * h;

            sum -= a * b;
            sum -= b * c;
            sum -= c * d;
            sum -= d * e;
            sum -= e * f;
            sum -= f * g;
            sum -= g * h;

            sum -= a * b;
            sum -= b * c;
            sum -= c * d;
            sum -= d * e;
            sum -= e * f;
            sum -= f * g;
            sum -= g * h;

            sum -= a * b;
            sum -= b * c;
            sum -= c * d;
            sum -= d * e;
            sum -= e * f;
            sum -= f * g;
            sum -= g * h;

            sum -= a * b;
            sum -= b * c;
            sum -= c * d;
            sum -= d * e;
            sum -= e * f;
            sum -= f * g;
            sum -= g * h;
        }
        printf("%d\n", sum);
    }
    
} 