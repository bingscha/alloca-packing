#include <stdio.h>

int main() {
    short a = 1, b = 2, c = 3, d = 4;
    long sum = 0;

    for (int i = 0; i < 10000000; ++i) {
        int to_add = 0;
        to_add += a++;
        to_add += b++;
        to_add += c++;
        to_add += d++;

        sum += to_add;
    }
    printf("%ld\n", sum);
}