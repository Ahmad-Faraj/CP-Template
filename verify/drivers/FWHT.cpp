// Driver only.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    int len = 1 << n;
    vector<ll> a(len), b(len);
    for (auto &x : a) cin >> x;
    for (auto &x : b) cin >> x;
    vector<ll> c = convolve(a, b, WHT_XOR, 998244353);
    string out;
    for (int i = 0; i < len; i++) out += to_string(c[i]) + (i + 1 < len ? " " : "");
    cout << out << "\n";
}
