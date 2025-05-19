#include <bits/stdc++.h>

using namespace std;

#define ll long long
#define PI acos(-1)
#define all(x) x.begin(),x.end()
#define clr(v, d) memset(v, d, sizeof(v))
#define endl "\n"
#define sz(x) (int)x.size()
#define vl vector<ll>
#define pb push_back

void M7MDAN_FARAG() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    #ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    #endif
}
int n,m,k;
int vis[30][30];
vector<int>r,c;
// knight moves
int dx[] = {1,1,2,2,-1,-1,-2,-2};
int dy[] = {2,-2,1,-1,2,-2,1,-1};
bool valid(int x, int y){
    return x >= 1 && x <= n && y >= 1 && y <= n;
}

int ret = 0;
void rec(int idx, int cnt){
    if(idx == m){
        ret = max(ret, cnt);
        return;
    }

    // leave
    rec(idx+1, cnt);

    int x = r[idx];
    int y = c[idx];
    if(!vis[x][y]){
        for(int k=0; k<8; k++){
            int nx = x + dx[k];
            int ny = y + dy[k];
            if(valid(nx, ny)){
                vis[nx][ny] = 1;            
            }
        }
        vis[x][y] = 1;
        rec(idx+1, cnt+1);
        for(int k=0; k<8; k++){
            int nx = x + dx[k];
            int ny = y + dy[k];
            if(valid(nx, ny)){
                vis[nx][ny] = 0;            
            }
        }
        vis[x][y] = 0;
    }
    return;
}

void solve() {
    cin >> n >> m;
    for(int i = 0; i < m; i++){
        int x,y;
        cin >> x >> y;
        r.pb(x);
        c.pb(y);
        vis[x][y] = 1;
    }
    // for(int i=1; i<=n; i++){
    //     for(int j=1; j<=n; j++){
    //         cout << vis[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    int ans = 0;
    for(int i=0; i<m; i++){
        int x = r[i];
        int y = c[i];
        if(!vis[x][y]){
            continue;
        }
        int cnt = 0;
        int a=0, b=0;
        for(int k=0; k<8; k++){
            int nx = x + dx[k];
            int ny = y + dy[k];
            if(valid(nx, ny)){
                if(vis[nx][ny]){
                    cnt++;
                    a = nx;
                    b = ny;
                }
            }
        }
        if(cnt >= 2){
            ans++;
            vis[x][y] = 0;
        }
        else if(cnt == 1){
            int cntsec = 0;
            for(int k=0; k<8; k++){
                int nx = a + dx[k];
                int ny = b + dy[k];
                if(valid(nx, ny)){
                    if(vis[nx][ny]){
                        cntsec++;
                    }
                }
            }
            if(cntsec > cnt){
                vis[a][b] = 0;
            }
            else{
                vis[x][y] = 0;
            }
            ans++;
        }
    }
    cout << ans << endl;
    // for(int i=1; i<=n; i++){
    //     for(int j=1; j<=n; j++){
    //         cout << vis[i][j] << " ";
    //     }
    //     cout << endl;
    // }
}



signed main() {
    M7MDAN_FARAG();
    ll t = 1;
    // cin >> t;

    while(t--) {
        solve();
    }
        
}