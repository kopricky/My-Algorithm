#include "../header.hpp"

#define getchar getchar_unlocked
#define putchar putchar_unlocked

// int の入力および出力の高速化

inline int in() {
    int n, c, flag = 1;
    while ((c = getchar()) < '0') if (c == EOF) return -1; else flag = -1;
    n = c - '0';
    while ((c = getchar()) >= '0') n = n * 10 + c - '0';
    return n * flag;
}

inline void out(int n) {
    short res[11], i = 0;
    if(n < 0) putchar('-'), n = -n;
    do { res[i++] = n % 10, n /= 10; } while (n);
    while (i) putchar(res[--i] + '0');
    putchar('\n');
}
