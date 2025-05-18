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

typedef vector<vector<int>> Matrix;

Matrix Multiply_Matrix(Matrix &a, Matrix &b) {
    bool Matrices_cannot_be_empty = false, Dimentions_are_not_valid = false;
    if (a.empty() or b.empty())     assert(Matrices_cannot_be_empty);
    if (a[0].size() != b.size())    assert(Dimentions_are_not_valid);

    Matrix Ans(a.size(), vector<int>(b.front().size()));

    for (int i = 0; i < int(a.size()); i++)
        for (int j = 0; j < int(b[0].size()); j++)
            for (int k = 0; k < int(a[0].size()); k++)
                Ans[i][j] += 1LL * a[i][k] * b[k][j] % MOD, Ans[i][j] %= MOD;

    return Ans;
}

Matrix fastpower(Matrix &base, ll po) {
    int baseSZ = base.size();
    // initialize answer to the identity matrix
    Matrix ANS(baseSZ, vector<int>(baseSZ));
    for (int i = 0; i < baseSZ; i++)
        ANS[i][i] = 1;

    while (po){
        if (po & 1)
            ANS = Multiply_Matrix(ANS, base);
        base = Multiply_Matrix(base, base);
        po >>= 1;
    }

    return ANS;
}

/* Matrix Initial_Mat -> Where we put the base cases */
/* Matrix Transition_Mat -> the matrix that represents the transition */
/* Matrix Transition_Mat_After_pow = fastpower(Transition_Mat, n - a) */
/* Matrix Final_answer = Multiply_Matrix(Initial_Mat, Transition_Mat_After_pow); */

void SOLVE(int ts_ctr) {
    // Fibonacci Example
    ll n;
    cin >> n;

    Matrix Initial_mat = {
        {0, 1},
        {0, 0}
        };
    
    Matrix Transition_mat = {
        {0, 1},
        {1, 1}
    };

    Matrix Transition_Mat_After_pow = fastpower(Transition_mat, n);

    Matrix Answer = Multiply_Matrix(Initial_mat, Transition_Mat_After_pow);

    cout << Answer[0][0];
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
