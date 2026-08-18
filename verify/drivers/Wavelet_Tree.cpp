// Driver only. kth(l, r, k) is 1-based inclusive with k 1-based; the judge is 0-based, [l, r), k 0-based.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; i++) cin >> a[i];
    t.init(a + 1, a + n + 1, -MAXV, MAXV);
    string out;
    while (q--) {
        int l, r, k;
        cin >> l >> r >> k;
        out += to_string(t.kth(l + 1, r, k + 1)) + "\n";
    }
    cout << out;
}
