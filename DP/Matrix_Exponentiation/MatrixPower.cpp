#include "../../core.h"

/*
 * Topic: Math/DP - Matrix Exponentiation
 * Description: Template for matrix operations and exponentiation in O(rows^3 log(power)).
 * Useful for linear recurrences with massive N.
 * Input: Size/dimensions of the matrix.
 * Output: Exponentiated matrix.
 */

template <typename T = int, int MOD_VALUE = 1000000007> struct Matrix {
    vector<vector<T>> mat;
    int rows, cols;

    // O ( n ^ 2 )
    Matrix(int rows, int cols) : rows(rows), cols(cols), mat(rows + 5, vector<T>(cols + 5, 0)) {}

    // O ( n ^ 2 )
    Matrix(int size) : Matrix(size, size) {}

    // O ( n ^ 2 )
    Matrix(const vector<vector<T>> &grid)
        : rows(grid.empty() ? 0 : (int)grid.size()), cols(grid.empty() ? 0 : (int)grid[0].size()) {
        mat.assign(rows + 5, vector<T>(cols + 5, 0));
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                mat[i][j] = grid[i - 1][j - 1];
            }
        }
    }

    static Matrix get_identity(int size) {
        Matrix res(size, size);
        for (int i = 1; i <= size; i++) res.mat[i][i] = 1 % MOD_VALUE;
        return res;
    }

    // O ( 1 )
    static T add_modd(T a, T b) { return (a + b) % MOD_VALUE; }

    // O ( 1 )
    static T sub_modd(T a, T b) { return (a - b % MOD_VALUE + MOD_VALUE) % MOD_VALUE; }

    // O ( 1 )
    static T mult_modd(T a, T b) { return (T)(((__int128)a * b) % MOD_VALUE); }

    // O ( n ^ 2 )
    Matrix operator+(const Matrix &b) const {
        Matrix res(rows, cols);
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                res.mat[i][j] = add_modd(mat[i][j], b.mat[i][j]);
            }
        }
        return res;
    }

    // O ( n ^ 2 )
    Matrix operator-(const Matrix &b) const {
        Matrix res(rows, cols);
        for (int i = 1; i <= rows; i++) {
            for (int j = 1; j <= cols; j++) {
                res.mat[i][j] = sub_modd(mat[i][j], b.mat[i][j]);
            }
        }
        return res;
    }

    // O ( n ^ 3 )
    Matrix operator*(const Matrix &b) const {
        Matrix res(rows, b.cols);
        for (int i = 1; i <= rows; i++) {
            for (int k = 1; k <= cols; k++) {
                if (mat[i][k] == 0) continue;
                for (int j = 1; j <= b.cols; j++) {
                    res.mat[i][j] = add_modd(res.mat[i][j], mult_modd(mat[i][k], b.mat[k][j]));
                }
            }
        }
        return res;
    }

    // O ( n ^ 3 * log(power) )
    Matrix operator^(ll power) const {
        Matrix res = get_identity(rows);
        Matrix base = *this;

        while (power > 0) {
            if (power & 1) res = res * base;
            base = base * base;
            power >>= 1;
        }
        return res;
    }
};
