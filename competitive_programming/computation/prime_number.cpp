vector<int> prime;
bool is_prime[MAX_N];

void sieve(int n){
	for(int i=0;i<=n;i++){
		is_prime[i] = true;
	}
	is_prime[0] = is_prime[1] = false;
	prime.push_back(2);
	for(int i=3;i<=n;i+=2){
		if(is_prime[i]){
			prime.push_back(i);
			for(int j=2*i;j<=n;j+=i){
				is_prime[j] = false;
			}
		}
	}
}
