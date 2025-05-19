#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define int long long
#define all(x) x.begin(), x.end()
#define endl "\n"
#define vl vector<ll>
#define sz(x) (ll)x.size()
#define pb push_back
#define clr(x, y) memset(x, y, sizeof(x))
#define modo 1000000007 

void M7MDAN_FARAG() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    #ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    #endif
}

struct Line {

    mutable ll m, c, p;

    bool operator < (const Line &o) const { 
        return m < o.m; 
    }

    bool operator < (ll x) const { 
        return p < x; 
    }

    ll val(ll x) const { 
        return m * x + c; 
    }
};

template < typename T = int, T Mode = -1 > class CHT : multiset < Line, less < > > {
private:
    
    static constexpr T LLimit = numeric_limits < T > :: min(), RLimit = numeric_limits < T > :: max();

    T div(T a, T b) { // floored division
        return a / b - ((a ^ b) < 0 && a % b);
    }

    bool is_intersect(iterator x, iterator y) {
        if (y == end()) return x -> p = RLimit, 0;
        if (x -> m == y -> m) x -> p = (x -> c > y -> c ? RLimit : LLimit);
        else x -> p = div(y -> c - x -> c, x -> m - y -> m);
        return x -> p >= y -> p;
    }

public:
    
    void add(T m, T c) {
        m *= Mode, c *= Mode;
        auto z = insert({m, c, 0}), y = z++, x = y;
        while (is_intersect(y, z)) z = erase(z);
        if (x != begin() && is_intersect(--x, y)) is_intersect(x, y = erase(y));
        while ((y = x) != begin() && (--x) -> p >= y -> p) is_intersect(x, erase(y));
    }

    T query(T x) {
        assert(!empty());
        auto l = *lower_bound(x);
        return Mode * l.val(x);
    }

};

void solve() {


}

signed main() {
    M7MDAN_FARAG();

    ll t = 1;
    // cin >> t;
    while (t--) {
        solve();
    }
}
