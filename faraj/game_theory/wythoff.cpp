#include <bits/stdc++.h>
using namespace std;

const double GOLDEN_RATIO = (1 + sqrt(5.0)) / 2;

pair<int, int> get_kth_wythoff_pair(int k) {
    int a = int(k * GOLDEN_RATIO);
    int b = a + k;
    return {a, b};
}

bool is_wythoff_losing(int a, int b) {
    if (a > b) swap(a, b);
    int k = b - a;
    return a == int(k * GOLDEN_RATIO);
}

vector<pair<int, int>> wythoff_pairs(int N) {
    vector<pair<int, int>> result;
    for (int k = 0; k < N; ++k) {
        result.push_back(get_kth_wythoff_pair(k));
    }
    return result;
}

void generate_wythoff(int N) {
    for (int k = 0; k < N; ++k) {
        auto [a, b] = get_kth_wythoff_pair(k);
        cout << "(" << a << "," << b << ") ";
    }
    cout << "\n";
}

// Check if a move from (a,b) to (x,y) is valid
bool is_valid_wythoff_move(int a, int b, int x, int y) {
    if (x > a || y > b) return false;
    int da = a - x, db = b - y;
    return (da == 0 && db > 0) || (db == 0 && da > 0) || (da == db && da > 0);
}

// Generate all legal moves from a position (a, b)
vector<pair<int, int>> generate_wythoff_moves(int a, int b) {
    vector<pair<int, int>> moves;
    for (int i = 1; i <= a; ++i) moves.emplace_back(a - i, b);
    for (int i = 1; i <= b; ++i) moves.emplace_back(a, b - i);
    for (int i = 1; i <= min(a, b); ++i) moves.emplace_back(a - i, b - i);
    return moves;
}

int main() {
    int a, b;
    cin >> a >> b;

    if (is_wythoff_losing(a, b))
        cout << "Second player will win (losing position for first)\n";
    else
        cout << "First player will win (winning position)\n";

    return 0;
}
