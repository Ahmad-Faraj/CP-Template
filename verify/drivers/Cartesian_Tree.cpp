// Driver only. The template is a MAX-heap; the judge wants the smallest element at the root, so the
// driver negates the values. No template change - the tree of -a is the min-heap tree of a.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto &x : a) {
        cin >> x;
        x = -x;
    }
    CartesianTree ct;
    ct.build(a);
    string out;
    for (int i = 1; i <= n; i++) {
        int p = ct.parent[i];
        out += to_string(p == 0 ? i - 1 : p - 1) + (i < n ? " " : "\n");
    }
    cout << out;
}
