#include <bits/stdc++.h>

using namespace std;

int n, m;
int a[101][101];
bool visited[101][101];
int dx[] = {-1, 0 ,0, 1};
int dy[] = {0, -1, 1, 0};

void dFs(int i, int j){
	cout << i <<" "<< j<<endl;
	visited[i][j] = true;
	// Duyet cac dinh ke
	for(int k = 0;k<4;k++){
		int i1 = i + dx[k];
		int j1 = j + dy[k];
		if( i1 >= 1 && j1 <= n && j1 >= 1 && j1 <= n && a[i1][j1]== 1 && !visited[i1][j1]){
			cout<<"Thanh phan lien thong thu "<<k+1<<endl;
			dFs(i1, j1);	
		}
	}
}

int main(){
	cin>>n>>m;
	for(int i=1; i <= n;i++){
		for(int j =1; j<=n ;j++){
			cin>>a[i][j];
		}
	}
	memset(visited, false, sizeof(visited));
	int cnt =0 ;
	for(int i=1; i<= n;i++){
		for(int j =1; j<=n;j++){
			if(a[i][j] == 1 && !visited[i][j]){
				dFs(i,  j);
				++cnt;
			}
		}
	}
	cout<<cnt <<endl;
	
	return 0;
}