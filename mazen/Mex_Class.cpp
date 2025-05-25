#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
#define ld long double
#define int long long
#define nl "\n"
#define oo 1e9 + 1
#define OO 1e18 + 1
#define sp ' '
#define sz(x) (int)(x.size())
#define MOD 1000000007
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace __gnu_pbds;
using namespace std;
void fastio()
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    #endif
}
//!!!! if time limit -> use set
//!!!! if time limit -> use set
//!!!! if time limit -> use set
//!!!! if time limit -> use set
template <typename T = int, const int N = 100005>
class MEX{
    public:
    MEX() {};
    bitset<N> mex;
    vector<int> cnt;
    inline void init(){
        cnt.assign(N, 0);
        for(int i = 0; i < N; i++)
            mex[i] = 1;
    }
    inline void add(const T &x){
        insert(x);
    }
    inline void remove(const T &x){
        erase(x);
    }
    inline int get_mex(){
        return calculate_mex();
    }
    private:
    inline void insert(const T &x){
        if(x > N)
            return;
        cnt[x]++;
        if(cnt[x] == 1)
            mex[x] = 0;
    }
    inline void erase(const T &x){
        if(x > N)
            return;
        cnt[x]--;
        if(cnt[x] == 0)
            mex[x] = 1;
    }
    inline int calculate_mex(){
        return mex._Find_first();
    }
};
void solve(int tc)
{
    
}
signed main(void)
{
    fastio();
    int tc = 1;
    //cin >> tc;
    int i = 1;
    while (tc--)
    {
        // cout<<"Case #"<<i<<": ";
        solve(i++);
    }
    return 0;
}