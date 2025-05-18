#include <bits/stdc++.h>

using namespace std;

#define cin2d(_$)for(auto&$:_$)for(auto&_:$)cin>>_;
#define cout2d(_$)for(auto&$:_$){for(auto &_:$)cout<<_<<" ";cout<<"\n";}
#define cout_map(_$) for(auto&[$,_]:_$)cout<<$<<" "<<_<<"\n";
#define cin(_$)for(auto&_:_$)cin >> _;
#define cout(_$){for(auto&_:_$)cout<<_ <<" ";cout<<"\n";}
#define all(_$) _$.begin(), _$.end()
#define unq(_$){sort(all(_$));_$.resize(unique(all(_$))-_$.begin());}
#define dg($$)  cout << #$$ << ": " << ($$) << "\n"
#define myCeil(N, M) ((N) / (M) + !!((N) % (M)))
#define sz(_$) (int)(_$.size())
typedef long long ll;
constexpr int MOD = 1e9 + 7; constexpr double EPS = 1e-9;



void SOLVE(int ts_ctr) {
    
}

signed main() {
    // cout << fixed << setprecision(10);
ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
    int _t_ = 1;
    // cin >> _t_;
    for (int tc=1;tc<=_t_; tc++)SOLVE(tc);
    /* cerr<<"Time Taken: "<<(float)clock()/CLOCKS_PER_SEC<<" Secs"<<"\n"; */
}
