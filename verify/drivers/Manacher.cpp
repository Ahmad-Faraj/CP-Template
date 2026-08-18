// Driver only. Judge centre i is a character when i is even and a gap when odd; getLongest takes
// the character index plus whether the palindrome is odd-length.
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    manacher m(s);
    int n = (int)s.size();
    string out;
    for (int i = 0; i < 2 * n - 1; i++) {
        ll len = (i % 2 == 0) ? m.getLongest(i / 2, true) : m.getLongest((i - 1) / 2, false);
        out += to_string(len) + (i + 2 < 2 * n ? " " : "\n");
    }
    cout << out;
}
