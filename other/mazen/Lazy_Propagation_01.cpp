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
// lazy propagation range change point query
// used for problems like 
// 1 l r x y   change all elements in range from l to r has value x to y
// 2 idx       query the value at index idx
struct LazyBinTreeUniversal {
    using T = int;
    using TMod = std::array<uint8_t, 101>;
    size_t size;
    std::vector<T> t;
    std::vector<TMod> mod;
    size_t h;
 
    LazyBinTreeUniversal(size_t N) :
        size(N),
        t(2 * N),
        mod(N, modInit()),
        h(32 - __builtin_clz(N)) { }
 
    LazyBinTreeUniversal(const std::vector<T>& other) :
        size(other.size()),
        t(2 * other.size()),
        mod(other.size(), modInit()),
        h(32 - __builtin_clz(other.size())) {
        std::copy(other.begin(), other.end(), t.begin() + size);
    }
 
    void apply(size_t p, const TMod& up_mod) {
        if (p < size) {
            if (mod[p][0] == 0) {
                mod[p] = up_mod;
            } else {
                mod[p] = combineMod(mod[p], up_mod);
            }
        } else {
            t[p] = combineValue(t[p], up_mod);
        }
    }
 
    void push(size_t p) {
        for (size_t s = h; s > 0; s--) {
            size_t pos = p >> s;
            if (mod[pos][0] != 0) {
                apply(2 * pos, mod[pos]);
                apply(2 * pos + 1, mod[pos]);
                mod[pos] = modInit();
            }
        }
    }
 
    void modify(size_t l, size_t r, const TMod& value) {
        if (l == r) return;
        l += size;
        r += size;
        push(l);
        push(r - 1);
        while (l < r) {
            if (l & 1) {
                apply(l, value);
                l++;
            }
            if (r & 1) {
                r--;
                apply(r, value);
            }
            l /= 2;
            r /= 2;
        }
    }
 
    T query(size_t p) {
        p += size;
        push(p);
        return t[p];
    }
  private:
 
    TMod combineMod(const TMod& mod, const TMod& up_mod) {
        TMod result = {};
        for (size_t i = 0; i < result.size(); i++) {
            result[i] = up_mod[mod[i]];
        }
        return result;
    }
 
    T combineValue(T value, const TMod& up_mod) {
        return up_mod[value];
    }
 
    T init() {
        return 0;
    }
 
    TMod modInit() {
        TMod result = {};
        std::iota(result.begin(), result.end(), 0);
        return result;
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