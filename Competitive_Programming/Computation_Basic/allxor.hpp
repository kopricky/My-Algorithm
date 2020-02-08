#include "../header.hpp"

// 0 ~ val までの全 xor
long long allxor(long long val) {
	return (val & 1LL) ? ((val & 3) == 1) : (val ^ (((val ^ 1) & 3) == 3));
}
