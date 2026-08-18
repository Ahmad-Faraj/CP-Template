// Driver only. The judge gives a parent array with p_i < i, 0-based; the template is 1-based.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    LCA t(n);
    for (int i = 1; i < n; i++) {
        int p;
        cin >> p;
        t.add_edge(p + 1, i + 1);
    }
    t.build(1);
    string out;
    while (q--) {
        int u, v;
        cin >> u >> v;
        out += to_string(t.get_lca(u + 1, v + 1) - 1) + "\n";
    }
    cout << out;
}
