#include <bits/stdc++.h>

using namespace std;

// Thuat toan Dijkstra (tim duong di ngan nhat)
int n,m,s,t;
int a[101][101];
vector <pair<int, int>> adj[101];
bool visited[101];
int pre[101]; // Luu cac dinh cha

void Nhap(){
	cin>>n>>m>>s>>t;
	for(int i=0;i<m;i++){
		int x,y,w;
		cin>>x>>y>>w;
		adj[x].push_back({y,w});
		adj[y].push_back({x,w});
	}
}




const int INF = 1e9;
void Dijkstra(int s, int t){
	// Mang luu khoang cach duong di
	vector<long long> d(n +1, INF);
	d[s] = 0;
	pre [s] =s;
	priority_queue< pair<int, int> , vector<pair<int,int>> , greater<pair<int, int>> > Q;
	// Luu {khoang cach,dinh }
	Q.push({0,s});
	while( !Q.empty()){
		// Chon ra dinh co khoang cach tu s nho nhat
		pair<int, int> top = Q.top();
		Q.pop();
		int u = top.second;
		int kc = top.first;
		if( kc > d[u]){
			continue;
		}
		// Relaxation : cap nhat khoang cach tu s cho toi moi dinh ke voi s
		for( auto it : adj[u]){
			int v = it.first;
			int w = it.second;
			if( d[v] > d[u] + w){
				d[v] = d[u] + w;
				Q.push({d[v], v});
				pre [v] = u; // truoc v la u
			}
		}	
	}
//	for(int i = 1; i <= n; i++){
//		cout<< d[i] <<" ";
//	}
	cout<< d[t] <<endl;
	vector <int> path;
	while(1){
		path.push_back(t);
		if( t == s){
			break;
		}
		t = pre[t];
	}
	reverse(begin(path), end(path));
	for(auto x : path){
		cout<< x<< " ";
	}
}

int main(){
	Nhap();
	Dijkstra(s,t);
	return 0;
}

