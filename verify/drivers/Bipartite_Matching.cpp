// Driver only. The template is 1-based on both sides; the judge is 0-based.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int l, r, m;
    cin >> l >> r >> m;
    Bipartite_Matching bm(l, r);
    for (int i = 0, a, b; i < m; i++) {
        cin >> a >> b;
        bm.add_edge(a + 1, b + 1);
    }
    string out = to_string(bm.maximum_matching()) + "\n";
    for (auto [u, v] : bm.pairs()) out += to_string(u - 1) + " " + to_string(v - 1) + "\n";
    cout << out;
}
