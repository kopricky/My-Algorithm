//1~valまでの全xor
ll allxor(ll val) {
	int i;
	ll ret=((val+1)/2)%2;
	for(i=0;i<=60;i++) if((val>>i)%2) ret |= (1^(val%2))<<i;
	return ret;
}
