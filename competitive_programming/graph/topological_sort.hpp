#include "../header.hpp"

#define MAX_N 100005

//バグはないけどtopological_sort_queueの方を使った方がいいかも
vector<int> G[MAX_N];
int color[MAX_N];
deque<int> L;
bool flag;	//閉路判定

void visit(int u)
{
	if(color[u] == 0){
		flag = true;	//DAGでない
		return;
	}
	if(color[u] == 1){
		return;
	}
	color[u] = 0;
	for(int i=0;i<(int)G[u].size();i++){
		visit(G[u][i]);
	}
	color[u] = 1;
	L.push_front(u);
}

int main()
{
	int n,m;
	scanf("%d%d",&n,&m);
	rep(i,m){
		int u,v;
		scanf("%d%d",&u,&v);
		G[u].push_back(v);
	}
	rep(i,n){
		color[i] = -1;
	}
	rep(i,n){
		if(color[i] == -1){
			flag = false;
			visit(i);
		}
		if(flag){
			break;
		}
	}
	rep(i,n){
		printf("%d",L[i]);
	}
}
