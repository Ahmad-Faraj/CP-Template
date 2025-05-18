#include <bits/stdc++.h>

using namespace std;

#define cout_2d(vec,n,m)for(int Ii=0;Ii<n;Ii++,cout<<"\n")for(int Jj=0;Jj<m&&cout<<vec[Ii][Jj]<<" ";Jj++);
#define cin_2d(vec,n,m)for(int iii=0;iii<n;iii++)for(int jjj=0;jjj<m&&cin>>vec[iii][jjj];jjj++);
#define cout_map(mp) for(auto& [fff, sss] : mp) cout << fff << "  " << sss << "\n";
#define cout(vec) for(auto& iii : vec) cout << iii << " "; cout << "\n";
#define cin(vec) for(auto& iii : vec) cin >> iii
#define to_decimal(bin) stoll(bin, nullptr, 2)
#define unq(VEC) sort(all(VEC)); VEC.resize(unique(all(VEC)) - VEC.begin());
#define ll long long
#define myCeil(N, M) ((N) / (M) + !!((N) % (M)))
#define dg(DG)   cout << #DG << ": " << (DG) << "\n"
#define dg1(DG) cout << #DG << ": " << (DG) << " || ";
#define dg3(DG1,DG2,DG3) {dg2(DG1, DG2) dg1(DG3)}
#define dg2(DG1, DG2) {dg1(DG1) dg1(DG2)}
#define dg4(DG1,DG2,DG3,DG4) {dg3(DG1,DG2,DG3) dg1(DG4)}
#define sz(x) (int)(x.size())
#define all(vec) vec.begin(), vec.end()
#define rall(vec) vec.rbegin(), vec.rend()
#define ff first
#define ss second
#define el "\n"
constexpr long long OO = 1 << 30, OOL = 1LL << 60, MOD = 1e9 + 7;
constexpr double EPS = 1e-9;
string get_binary(long long num){if(!num)return"";return get_binary(num/2)+(num%2?"1":"0");}
bool is_subseq(string p, string w){int szp = p.size(), szw = w.size(), ip=0, iw=0;
for (; ip < szp && iw < szw; iw++){if (p[ip] == w[iw]){ip++;}}  return ip == szp;}
vector<pair<int,int>>dxy4={{1,0},{0,1},{0,-1},{-1,0}};
vector<pair<int,int>>dxy8={{1,-1}, {0,-1}, {-1,-1}, {1,0}, {-1,0}, {1,1}, {0,1}, {-1,1}};
vector<pair<int,int>>dxyk={{-1,-2}, {-2,-1}, {1,-2}, {2,-1}, {-2,1}, {-1,2}, {1,2}, {2,1}};

enum TYPE {MIN, MAX, SUM, GCD};
template <typename T, int Q_BASE, TYPE queryType> struct Sparse {
    T table[200002][22], arr[200002];                       /* Size of the arrays */
    T InitialElement = INT_MAX;

    T opr(T &a, T &b) {
        if (queryType == TYPE::MIN)
            return min(a, b);
        if (queryType == TYPE::MAX)
            return max(a, b);
        if (queryType == TYPE::GCD)
            return gcd(a, b);
        if (queryType == TYPE::SUM)
            return a + b;
        return a + b;
    }

    int n = 0;

    void preCalc(int n) {
        this->n = n;
        for (int i = 0; i < n; i++)
            Sparse::table[i][0] = Sparse::arr[i];
        for (int j = 1; j < __lg(n) + 1; j++)
            for (int i = 0; i + (1 << j) - 1 < n; i++)
                Sparse::table[i][j] = opr(Sparse::table[i][j - 1], Sparse::table[i + (1 << (j - 1))][j - 1]);
    }

    T getAns(int l, int r) {
        l -= Q_BASE, r -= Q_BASE;
        return queryType == TYPE::SUM ? getAns1(l, r) : getAns2(l, r);
    }
private:
    // get Ans in O(log)
    T getAns1(int l, int r) {
        T ans = InitialElement;
        int len = r - l + 1, idx = l;
        for (int bit = __lg(n) + 1; bit >= 0; bit--)
            if (len & (1 << bit))
                ans = opr(ans, Sparse::table[idx][bit]), idx += (1 << bit);
        return ans;
    }

    // get Ans in O(1)
    T getAns2(int l, int r) {
        int len = r - l + 1;
        int LG = __lg(len);
        return opr(Sparse::table[l][LG], Sparse::table[r - (1 << LG) + 1][LG]);
    }
};

Sparse < int, 0, TYPE::MIN> sp;

void SOLVE(int ts_ctr) {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
        cin >> sp.arr[i];

    sp.preCalc(n);

    int q, l, r;
    cin >> q;
    while (q--) {
        cin >> l >> r;
        cout << sp.getAns(l, r) << "\n";
    }
}

signed main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#if !defined(ONLINE_JUDGE)
    freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
#endif
    // cout << fixed << setprecision(10);
    int testcases = 1;
    // cin >> testcases;
    for (int test_ctr = 1; test_ctr <= testcases; test_ctr++)   SOLVE(test_ctr);
    /* cerr<<"Time Taken: "<<(float)clock()/CLOCKS_PER_SEC<<" Secs"<<"\n"; */
}
