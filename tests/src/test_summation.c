#include <stdio.h>

int main() {
    short a = 1, b = 2, c = 3, d = 4;
    int sum = 0;

    for (long i = 0; i < 100000000; ++i) {
        int to_add = 0;
        to_add += a++;
        to_add += b++;
        to_add += c++;
        to_add += d++;

        sum += to_add;

        sum *= a++;
        sum *= b++;
        sum *= c++;
        sum *= d++;
    }
    printf("%ld\n", sum);
}