#include "../header.hpp"

vector<ll> dev;
vector<int> id;

void prime_factor(ll N){
	for(ll i=2;i*i<=N;i++){
		int cnt = 0;
		while(N%i == 0){
			cnt++;
			N /= i;
		}
		if(cnt){
			dev.push_back(i);
			id.push_back(cnt);
		}
	}
	if(N != 1){
		dev.push_back(N);
		id.push_back(1);
	}
	return;
}
