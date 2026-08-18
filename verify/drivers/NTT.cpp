// Driver only. multiply defaults to mod 998244353, which is this problem's modulus.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<ll> a(n), b(m);
    for (auto &x : a) cin >> x;
    for (auto &x : b) cin >> x;
    vector<ll> c = multiply(a, b);
    string out;
    for (size_t i = 0; i < c.size(); i++) out += to_string(c[i]) + (i + 1 < c.size() ? " " : "");
    cout << out << "\n";
}
