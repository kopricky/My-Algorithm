#include "../header.hpp"

//1~valまでの全xor
long long allxor(long long val) {
	int i;
	long long ret=((val+1)/2)%2;
	for(i=0;i<=60;i++) if((val>>i)%2) ret |= (1^(val%2))<<i;
	return ret;
}
