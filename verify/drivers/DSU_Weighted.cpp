// Driver only. The judge asserts a_u = a_v + x (mod 998244353); unite(a, b, w) asserts
// value[b] - value[a] = w, so the assertion is unite(v, u, x) and the query is diff(v, u).
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    DSU_Weighted<ll, 0, 998244353> dsu(n);
    string out;
    while (q--) {
        int t, u, v;
        cin >> t >> u >> v;
        if (t == 0) {
            ll x;
            cin >> x;
            out += dsu.unite(v, u, x) ? "1\n" : "0\n";
        } else {
            out += (dsu.same(u, v) ? to_string(dsu.diff(v, u)) : string("-1")) + "\n";
        }
    }
    cout << out;
}
