// Driver only. The template is 1-based on both workers and jobs; the judge's matrix is 0-based.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    Hungarian h(n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            ll c;
            cin >> c;
            h.set_cost(i + 1, j + 1, c);
        }
    ll total = h.minimum_cost();
    vector<int> p(n);
    for (auto [w, j] : h.pairs()) p[w - 1] = j - 1;
    string out = to_string(total) + "\n";
    for (int i = 0; i < n; i++) out += to_string(p[i]) + (i + 1 < n ? " " : "");
    cout << out << "\n";
}
