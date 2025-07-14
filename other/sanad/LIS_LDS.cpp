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

// To calculate LIS (Longest Increasing Subsequence) pass strictly_increasing = true,
// To calculate LNDS (Longest Non Decreasing Subsequence) pass strictly_increasing = false
vector < int > get_LIS(vector <int> &v, bool STRICTLY_INCREASING = true) {
    int n = v.size();
    vector < int > temp = {v.front()};
    vector < int > LIS(n, 1);

    // calc the length of LIS at each index
    for (int i = 1, myIdx; i < n; i++) {
        if (STRICTLY_INCREASING)    // some thing like this [1, 3, 5, 6, 12, 43]
            myIdx = lower_bound(temp.begin(), temp.end(), v[i]) - temp.begin();
        else    // something like this [1, 3, 3, 7, 8, 8, 8, 8, 9]
            myIdx = lower_bound(temp.begin(), temp.end(), v[i]) - temp.begin();
    
        if (myIdx == int(temp.size())) { // its greater than all previous elements, then add it at the end
            temp.emplace_back(v[i]);    
        }
        else {  // then replace it with the first element greater than it
            temp[myIdx] = v[i];
        }

        // Update the LIS seen so far
        LIS[i] = temp.size();
    }

    return LIS;
}

// LDS is basicly a reversed LIS
vector < int > get_LDS(vector <int> &v, bool STRICTLY_DECREASING = true) {
    int n = v.size();
    vector < int > temp = {v.back()};
    vector < int > LDS(n, 1);

    // calc the length of LDS at each index
    for (int i = n - 2, myIdx; i >= 0; i--) {
        if (STRICTLY_DECREASING)    // some thing like this [43, 12, 6, 5, 3, 1]
            myIdx = lower_bound(temp.begin(), temp.end(), v[i]) - temp.begin();
        else    // something like this [9, 8, 8, 8, 8, 7, 3, 3, 1]
            myIdx = lower_bound(temp.begin(), temp.end(), v[i]) - temp.begin();

        if (myIdx == int(temp.size())) { // its greater than all next elements, then add it at the end
            temp.emplace_back(v[i]);    
        }
        else {  // then replace it with the first element greater than it
            temp[myIdx] = v[i];
        }

        // Update the LDS seen so far
        LDS[i] = temp.size();
    }


    return LDS;
}


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
