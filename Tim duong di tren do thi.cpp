#include <bits/stdc++.h>
using namespace std;

int n,m, s, t;
int a[101][101];
vector <int> adj[101];
bool visited[101];
int parent[101];

void dFs(int u){
	visited[u] = true;
	for(auto x : adj[u]){
		if( visited[x] == false){
			//Ghi nhan cha cua x la u
			parent[x] = u;
			dFs(x);
		}
	}
}

void paTh(int s, int t){
	memset(visited, false, sizeof(visited));
	memset(parent, 0 , sizeof(parent));
	dFs(s);
	if(!visited[t]){
		cout<<" Khong co duong di"<<endl;
	}else{
		//Truy vet duong di
		vector<int> path;
		//Bat dau tu dinh ket thuc nguoc lai
		while(t != s){
			path.push_back(t);
			// Lan nguoc lai
			t = parent[t];
		}
		path.push_back(s);
		reverse(path.begin(), path.end());
		for(auto x : path){
			cout<<x<<"  ";
		}
	}
}

int main(){
	cin>>n>>m;
	for(int i=0;i < m;i++){
		int x,y;
		cin>>x>>y;
		adj[x].push_back(y);
		adj[y].push_back(x);
	}
	cout<<"Nhap diem bat dau va ket thuc:";
	cin>>s>>t;
	cout<<"Duong di tu dinh "<<s<<" den dinh "<<t<<" la :";
	paTh(s,t);
	
	
	return 0;
}