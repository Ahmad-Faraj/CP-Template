#include<bits/stdc++.h>
using namespace std;
#define ll long long

const double EPS = 1e-9;

// Solve system of N linear equations.
// aug -> Augmented Matrix : Nx(N+1) matrix with last column for constants.
// Returns empty vector if matrix is singular (no unique solution).
vector<double> gauss_solve(int n, vector<vector<double>> aug) {
    for (int i = 0; i < n; i++) {
        int l = i;
        for (int j = i + 1; j < n; j++)
            if (fabs(aug[j][i]) > fabs(aug[l][i]))
                l = j;

        if (fabs(aug[l][i]) < EPS) return {}; // Singular matrix

        for (int k = i; k <= n; k++)
            swap(aug[i][k], aug[l][k]);

        for (int j = 0; j < n; j++) {
            if (i != j) {
                double factor = aug[j][i] / aug[i][i];
                for (int k = i; k <= n; k++)
                    aug[j][k] -= aug[i][k] * factor;
            }
        }
    }
    
    vector<double> ans(n);
    for (int i = 0; i < n; i++) {
        ans[i] = aug[i][n] / aug[i][i];
    }
    return ans;
}