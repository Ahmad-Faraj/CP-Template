// Matrix Power
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
struct Matrix
{
    vector<vector<int>> mat;
    int n, m;
    Matrix(int n, int m)
    {
        this->n = n;
        this->m = m;
        mat.resize(n, vector<int>(m, 0));
    }
    Matrix(vector<vector<int>> &mat)
    {
        this->n = mat.size();
        this->m = mat[0].size();
        this->mat = mat;
    }
    Matrix identity(int n)
    {
        Matrix c(n, n);
        for (int i = 0; i < n; i++)
        {
            c.mat[i][i] = 1;
        }
        return c;
    }
    Matrix operator+(Matrix &b)
    {
        Matrix c(n, m);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                c.mat[i][j] = add_mod(mat[i][j], b.mat[i][j], MOD);
            }
        }
        return c;
    }
    Matrix operator-(Matrix &b)
    {
        Matrix c(n, m);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                c.mat[i][j] = sub_mod(mat[i][j], b.mat[i][j], MOD);
            }
        }
        return c;
    }
    Matrix operator*(Matrix &b)
    {
        Matrix c(n, b.m);
        for (int i = 0; i < n; i++)
        {
            for (int k = 0; k < m; k++)
            {
                for (int j = 0; j < b.m; j++)
                {
                    c.mat[i][j] = add_mod(c.mat[i][j], mult_mod(mat[i][k], b.mat[k][j], MOD), MOD);
                }
            }
        }
        return c;
    }
    Matrix operator^(int p)
    {
        Matrix c = identity(n);
        Matrix a = *this;
        while (p > 0)
        {
            if (p % 2)
            {
                c = c * a;
            }
            a = a * a;
            p /= 2;
        }
        return c;
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