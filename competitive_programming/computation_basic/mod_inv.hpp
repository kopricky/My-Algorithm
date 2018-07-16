#include "../header.hpp"

void extgcd(int a,int b, int& x,int& y)
{
	if(b != 0){
		extgcd(b,a%b,y,x);
		y -= (a/b)*x;
	}else{
		x = 1;
		y = 0;
	}
}

int mod_inverse(int a,int m)
{
	int x,y;
	extgcd(a,m,x,y);
	return (m + x % m) % m;
}
