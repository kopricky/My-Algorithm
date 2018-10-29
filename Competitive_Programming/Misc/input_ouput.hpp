#include "../header.hpp"

#define getchar getchar_unlocked
#define putchar putchar_unlocked

// int の入力および出力の高速化

inline int in() {
    int n = 0; short c; bool flag = false;
    if((c = getchar()) < '0') flag = true; else n = c - '0';
    while ((c = getchar()) >= '0') n = n * 10 + c - '0';
    return flag ? -n : n;
}

inline void out(int n) {
    short res[10], i = 0;
    if(n < 0) putchar('-'), n = -n;
    do { res[i++] = n % 10, n /= 10; } while (n);
    while (i) putchar(res[--i] + '0');
    putchar('\n');
}
