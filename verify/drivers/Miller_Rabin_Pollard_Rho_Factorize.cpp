// Driver only. The judge wants the factors ascending; prime_factors returns them unordered.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    cin >> q;
    string out;
    while (q--) {
        unsigned long long a;
        cin >> a;
        vector<unsigned long long> f = prime_factors(a);
        sort(f.begin(), f.end());
        out += to_string(f.size());
        for (unsigned long long x : f) out += " " + to_string(x);
        out += "\n";
    }
    cout << out;
}
