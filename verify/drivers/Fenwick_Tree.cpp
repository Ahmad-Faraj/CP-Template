// Driver only. FenwickTree is 1-based and sum(l, r) is inclusive; the judge is 0-based and [l, r).
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    FenwickTree<> bit(a);
    string out;
    while (q--) {
        int t, x, y;
        cin >> t >> x >> y;
        if (t == 0)
            bit.add(x + 1, y);
        else
            out += to_string(bit.sum(x + 1, y)) + "\n";
    }
    cout << out;
}
