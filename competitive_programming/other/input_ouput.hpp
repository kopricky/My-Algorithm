#include "../header.hpp"

#define getchar getchar_unlocked
#define putchar putchar_unlocked

// int の入力および出力の高速化

int in() {
    int n, c;
    while ((c = getchar()) < '0') if (c == EOF) return -1;
    n = c - '0';
    while ((c = getchar()) >= '0') n = n * 10 + c - '0';
    return n;
}

void out(int n) {
    int res[11], i = 0;
    do { res[i++] = n % 10, n /= 10; } while (n);
    while (i) putchar(res[--i] + '0');
    putchar('\n');
}
