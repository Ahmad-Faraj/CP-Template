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

template < int THE_MAX_SIZE = 200010 >
struct Hashing {
private:
    ll fastpower(ll base, ll po, ll md) {
        ll ANS = 1;
        while (po){
            if (po & 1)
                ANS = (ANS * base) % md;
            base *= base, base %= md;
            po >>= 1;
        }
        return ANS;
    }

    ll mod_inv(ll Num, ll md){
        return fastpower(Num, md - 2, md);
    }

    /* pwPref => pr^1 + pr^2 + pr^3 + ..... */

    static constexpr int NNN = THE_MAX_SIZE + 10;

    int pw[NNN][2], pwInv[NNN][2], pwPref[NNN][2], pwInvPref[NNN][2], mod[2] = {int(1e9 + 7), int(1e9 + 9)};

public:
    void preCalcMod() {
        int pr[2] = {31, 37};
        for (int i = 0; i <= 1; i++)
            for (int j = 0; j <= 1; j++)
                pw[i][j] = pwInv[i][j] = 1;
        
        for (int j = 0; j <= 1; j++)
            pwPref[1][j] = pwInvPref[1][j] = 1;
        
        int prInv[2] = {(int)mod_inv(pr[0], mod[0]), (int)mod_inv(pr[1], mod[1])};

        for (int j = 0; j <= 1; j++)
            for (int i = 2; i < NNN; i++) {
                pw[i][j] = 1LL * pw[i - 1][j] * pr[j] % mod[j];
                pwPref[i][j] = (pwPref[i - 1][j] + pw[i][j]) % mod[j];
                
                pwInv[i][j] = 1LL * pwInv[i - 1][j] * prInv[j] % mod[j];
                pwInvPref[i][j] = (pwInvPref[i - 1][j] + pwInv[i][j]) % mod[j];
            }
    }


    int hashingPrefix[NNN][2];

    void calcHash(vector<int> v) {
        v.insert(v.begin(), 0);
        for (int j = 0; j < 2; j++)
            for (int i = 1; i < int(v.size()); i++) {
                hashingPrefix[i][j] = 1LL * v[i] * pw[i][j] % mod[j];
                hashingPrefix[i][j] += hashingPrefix[i - 1][j];
                hashingPrefix[i][j] %= mod[j];
            }
    }

    void calcHash(string s) {
        s.insert(s.begin(), '#');
        for (int j = 0; j < 2; j++)
            for (int i = 1; i < int(s.size()); i++) {
                hashingPrefix[i][j] = 1LL * (s[i] - 'a' + 1) * pw[i][j] % mod[j];
                hashingPrefix[i][j] += hashingPrefix[i - 1][j];
                hashingPrefix[i][j] %= mod[j];
            }
    }

    pair<int, int> getHashVal(int l, int r) {
        int firstVal = (hashingPrefix[r][0] - hashingPrefix[l - 1][0] + mod[0]) % mod[0] * 1LL * pwInv[l][0] % mod[0];
        int secondVal = (hashingPrefix[r][1] - hashingPrefix[l - 1][1] + mod[1]) % mod[1] * 1LL * pwInv[l][1] % mod[1];

        return make_pair(firstVal, secondVal);
    }

    pair<int, int> getHashVal(string s) {
        s.insert(s.begin(), '#');
        int firstVal = 0, secondVal = 0;
        for (int i = 1; i < int(s.size()); i++) {
            firstVal = (1LL * (s[i] - 'a' + 1) * pw[i][0] % mod[0] + firstVal) % mod[0];
            secondVal = (1LL * (s[i] - 'a' + 1) * pw[i][1] % mod[1] + secondVal) % mod[1];
        }
        return make_pair(firstVal, secondVal);
    }

    pair<int, int> getHashVal(vector<int> v) {
        v.insert(v.begin(), 0);
        int firstVal = 0, secondVal = 0;
        for (int i = 1; i < int(v.size()); i++) {
            firstVal = (1LL * v[i] * pw[i][0] % mod[0] + firstVal) % mod[0];
            secondVal = (1LL * v[i] * pw[i][1] % mod[1] + secondVal) % mod[1];
        }
        return make_pair(firstVal, secondVal);
    }

    // /* pr^l + pr^(l+1) + pr^(l+2) + ...... + pr^(r-1) + pr^r  */
    // pair<int, int> getRangeHash(int l, int r) {
    //     int firstVal = (pwPref[r][0] - pwPref[l - 1][0] + mod[0]) % mod[0];
    //     int secondVal = (pwPref[r][1] - pwPref[l - 1][1] + mod[1]) % mod[1];

    //     return make_pair(firstVal, secondVal);
    // }
};

/* ⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠ Here I pass the maximum size of the string or the vector ⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠ */
Hashing < 100000 > Ha;

void SOLVE(int ts_ctr) {
    // this problem wants me to calculate the number of distinct substrings of size ""k""
    /* https://vjudge.net/problem/SPOJ-ADACLEAN */
    
    int n, k;
    string s;
    cin >> n >> k >> s;
    Ha.calcHash(s);

    set < pair < int , int > > uniqueHashes;

    for (int i = 1; i + k - 1 <= (int)s.size(); i++)
        uniqueHashes.emplace(Ha.getHashVal(i, i + k - 1));

    cout << sz(uniqueHashes) << "\n";
}

signed main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#if !defined(ONLINE_JUDGE)
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
    /* ⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠ Precalculating  the Mod before the test cases ⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠⚠ */
    Ha.preCalcMod();
    // cout << fixed << setprecision(10);
    int testcases = 1;
    cin >> testcases;
    for (int test_ctr = 1; test_ctr <= testcases; test_ctr++)   SOLVE(test_ctr);
    /* cerr<<"Time Taken: "<<(float)clock()/CLOCKS_PER_SEC<<" Secs"<<"\n"; */
}
