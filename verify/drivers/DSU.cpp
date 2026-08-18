// Driver only. DSU is 0-based here via DSU<int, 0>; the judge numbers vertices 0..N-1.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    DSU<int, 0> dsu(n);
    string out;
    while (q--) {
        int t, u, v;
        cin >> t >> u >> v;
        if (t == 0)
            dsu.union_sets(u, v);
        else
            out += dsu.is_same_sets(u, v) ? "1\n" : "0\n";
    }
    cout << out;
}
