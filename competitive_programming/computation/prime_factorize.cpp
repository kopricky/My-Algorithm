vector<ll> dev;
vector<int> id;

void prime_factor(ll N)
{
	for(ll i=2;i*i<=N;i++){
		int cnt = 0;
		while(N%i == 0){
			cnt++;
			N /= i;
		}
		if(cnt){
			dev.pb(i);
			id.pb(cnt);
		}
	}
	if(N != 1){
		dev.pb(N);
		id.pb(1);
	}
	return;
}
