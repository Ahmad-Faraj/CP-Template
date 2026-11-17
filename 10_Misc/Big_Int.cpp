// Big Int: arbitrary-precision signed integers with + - * / %, comparisons, stream I/O and power.
// Use when: values outrun 64 bits and __int128 - huge powers or factorials, 100+ digit input.
// Usage: BigInt a("-1234567890123456789012345"), b = 42; cout << a * b + a / b << ' ' << a % b;
// Handles: negatives, zero, long long operands (a * v, a / v, a % v), divmod, gcd, power(a, k).
// Time: + - O(n) | * O(n^2), FFT once limbs(a) * limbs(b) > 1e6 | / % O(n^2) | n = digits / 9
// Indexing: limbs base 1e9 in a, least significant first; you never touch them directly
// Note: / and % truncate toward zero like C++ (-7 / 2 = -3, -7 % 2 = -1). a % (ll v) returns a ll
// and needs |v| <= 9e9. Dividing by zero is undefined. gcd returns a non-negative value.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct BigInt {
    static constexpr int DIG = 9, FDIG = 4, BASE = 1000000000, FBASE = 10000;
    using ld = long double;
    struct Cplx { // minimal complex number for the FFT
        ld x, y;
        Cplx(ld x = 0, ld y = 0) : x(x), y(y) {}
        Cplx operator+(const Cplx &c) const { return {x + c.x, y + c.y}; }
        Cplx operator-(const Cplx &c) const { return {x - c.x, y - c.y}; }
        Cplx operator*(const Cplx &c) const { return {x * c.x - y * c.y, x * c.y + y * c.x}; }
        Cplx &operator*=(const Cplx &c) { return *this = *this * c; }
        Cplx operator/(ld c) const { return {x / c, y / c}; }
    };

    int sgn;
    vector<int> a;
    BigInt() : sgn(1) {}
    BigInt(ll v) { *this = v; }
    BigInt(const string &s) { read(s); }
    BigInt &operator=(ll v) {
        sgn = v < 0 ? -1 : 1;
        unsigned long long u = v < 0 ? 0ULL - (unsigned long long)v : v; // LLONG_MIN is fine
        a.clear();
        for (; u > 0; u /= BASE) a.push_back(u % BASE);
        return *this;
    }
    void read(const string &s) {
        sgn = 1;
        a.clear();
        int k = 0;
        for (; k < (int)s.size() && (s[k] == '-' || s[k] == '+'); k++)
            if (s[k] == '-') sgn = -sgn;
        for (int i = (int)s.size() - 1; i >= k; i -= DIG) {
            int x = 0;
            for (int j = max(k, i - DIG + 1); j <= i; j++) x = x * 10 + s[j] - '0';
            a.push_back(x);
        }
        trim();
    }
    friend istream &operator>>(istream &in, BigInt &v) {
        string s;
        in >> s;
        v.read(s);
        return in;
    }
    friend ostream &operator<<(ostream &out, const BigInt &v) {
        if (v.sgn == -1 && !v.zero()) out << '-';
        out << (v.a.empty() ? 0 : v.a.back());
        for (int i = (int)v.a.size() - 2; i >= 0; --i) out << setw(DIG) << setfill('0') << v.a[i];
        return out;
    }

    bool operator<(const BigInt &v) const {
        if (sgn != v.sgn) return sgn < v.sgn;
        if (a.size() != v.a.size()) return (a.size() < v.a.size()) == (sgn > 0);
        for (int i = (int)a.size() - 1; i >= 0; i--)
            if (a[i] != v.a[i]) return a[i] * sgn < v.a[i] * sgn;
        return 0;
    }
    bool operator>(const BigInt &v) const { return v < *this; }
    bool operator<=(const BigInt &v) const { return !(v < *this); }
    bool operator>=(const BigInt &v) const { return !(*this < v); }
    bool operator==(const BigInt &v) const { return !(*this < v) && !(v < *this); }
    bool operator!=(const BigInt &v) const { return *this < v || v < *this; }
    friend int cmp_abs(const BigInt &x, const BigInt &y) {
        if (x.a.size() != y.a.size()) return x.a.size() < y.a.size() ? -1 : 1;
        for (int i = (int)x.a.size() - 1; i >= 0; --i)
            if (x.a[i] != y.a[i]) return x.a[i] < y.a[i] ? -1 : 1;
        return 0;
    }

    BigInt operator-() const {
        BigInt res = *this;
        if (!zero()) res.sgn = -sgn;
        return res;
    }
    void add_abs(const BigInt &v) {
        if (a.size() < v.a.size()) a.resize(v.a.size(), 0);
        for (int i = 0, carry = 0; i < (int)max(a.size(), v.a.size()) || carry; ++i) {
            if (i == (int)a.size()) a.push_back(0);
            a[i] += carry + (i < (int)v.a.size() ? v.a[i] : 0);
            carry = a[i] >= BASE;
            if (carry) a[i] -= BASE;
        }
    }
    void sub_abs(const BigInt &v) { // needs |*this| >= |v|
        for (int i = 0, carry = 0; i < (int)v.a.size() || carry; ++i) {
            a[i] -= carry + (i < (int)v.a.size() ? v.a[i] : 0);
            carry = a[i] < 0;
            if (carry) a[i] += BASE;
        }
        trim();
    }
    BigInt &operator+=(const BigInt &v) {
        if (sgn == v.sgn) add_abs(v);
        else if (cmp_abs(*this, v) >= 0) sub_abs(v);
        else {
            BigInt vv = v;
            swap(*this, vv);
            sub_abs(vv);
        }
        return *this;
    }
    BigInt &operator-=(const BigInt &v) {
        if (sgn != v.sgn) add_abs(v);
        else if (cmp_abs(*this, v) >= 0) sub_abs(v);
        else {
            BigInt vv = v;
            swap(*this, vv);
            sub_abs(vv);
            sgn = -sgn;
        }
        return *this;
    }
    friend BigInt operator+(BigInt l, const BigInt &r) { l += r; return l; }
    friend BigInt operator-(BigInt l, const BigInt &r) { l -= r; return l; }

    friend pair<BigInt, BigInt> divmod(const BigInt &a1, const BigInt &b1) { // {quot, rem}
        ll norm = BASE / (b1.a.back() + 1);
        BigInt a = a1.abs() * norm, b = b1.abs() * norm, q = 0, r = 0;
        q.a.resize(a.a.size());
        for (int i = (int)a.a.size() - 1; i >= 0; i--) {
            r *= BASE;
            r += a.a[i];
            ll s1 = r.a.size() <= b.a.size() ? 0 : r.a[b.a.size()];
            ll s2 = r.a.size() <= b.a.size() - 1 ? 0 : r.a[b.a.size() - 1];
            ll d = ((ll)BASE * s1 + s2) / b.a.back();
            r -= b * d;
            while (r < 0) r += b, --d;
            q.a[i] = d;
        }
        q.sgn = a1.sgn * b1.sgn;
        r.sgn = a1.sgn;
        q.trim();
        r.trim();
        return make_pair(q, r / norm);
    }
    BigInt operator/(const BigInt &v) const { return divmod(*this, v).first; }
    BigInt operator%(const BigInt &v) const { return divmod(*this, v).second; }
    void operator/=(const BigInt &v) { *this = *this / v; }
    void operator/=(ll v) {
        if (v >= BASE || v <= -BASE) {
            *this /= BigInt(v);
            return;
        }
        if (v < 0) sgn = -sgn, v = -v;
        ll rem = 0;
        for (int i = (int)a.size() - 1; i >= 0; --i) {
            ll cur = a[i] + rem * BASE;
            a[i] = (int)(cur / v);
            rem = cur % v;
        }
        trim();
    }
    BigInt operator/(ll v) const {
        BigInt res = *this;
        res /= v;
        return res;
    }
    ll operator%(ll v) const { // |v| <= 9e9, sign follows *this
        ll m = 0;
        for (int i = (int)a.size() - 1; i >= 0; --i) m = (a[i] + m * BASE) % v;
        return m * sgn;
    }
    void operator*=(ll v) {
        if (v >= BASE || v <= -BASE) {
            *this *= BigInt(v);
            return;
        }
        if (v < 0) sgn = -sgn, v = -v;
        ll carry = 0;
        for (int i = 0; i < (int)a.size() || carry; ++i) {
            if (i == (int)a.size()) a.push_back(0);
            ll cur = a[i] * v + carry;
            carry = cur / BASE;
            a[i] = (int)(cur % BASE);
        }
        trim();
    }
    BigInt operator*(ll v) const {
        BigInt res = *this;
        res *= v;
        return res;
    }

    static vector<int> convert_base(const vector<int> &a, int old_digits, int new_digits) {
        vector<ll> p(max(old_digits, new_digits) + 1);
        p[0] = 1;
        for (int i = 1; i < (int)p.size(); i++) p[i] = p[i - 1] * 10;
        vector<int> res;
        ll cur = 0;
        int cur_digits = 0;
        for (int i = 0; i < (int)a.size(); i++) {
            cur += a[i] * p[cur_digits];
            cur_digits += old_digits;
            while (cur_digits >= new_digits) {
                res.push_back((int)(cur % p[new_digits]));
                cur /= p[new_digits];
                cur_digits -= new_digits;
            }
        }
        res.push_back((int)cur);
        while (!res.empty() && !res.back()) res.pop_back();
        return res;
    }
    static int rev_incr(int a, int n) { // next index in bit-reversed order
        unsigned x = a, msk = n / 2, cnt = 0; // unsigned: the shift may run past bit 31
        while (x & msk) cnt++, x <<= 1;
        x &= msk - 1;
        x |= msk;
        while (cnt--) x >>= 1;
        return x;
    }
    static vector<Cplx> FFT(vector<Cplx> v, int dir = 1) {
        const ld PI = acosl(-1);
        Cplx wm, w, u, t;
        int n = v.size();
        vector<Cplx> V(n);
        for (int k = 0, a = 0; k < n; ++k, a = rev_incr(a, n)) V[a] = v[k] / ld(dir > 0 ? 1 : n);
        for (int m = 2; m <= n; m <<= 1) {
            ld ang = dir * 2 * PI / m;
            wm = Cplx(cosl(ang), sinl(ang));
            for (int k = 0; k < n; k += m) {
                w = 1;
                for (int j = 0; j < m / 2; ++j, w *= wm) {
                    u = V[k + j];
                    t = w * V[k + j + m / 2];
                    V[k + j] = u + t;
                    V[k + j + m / 2] = u - t;
                }
            }
        }
        return V;
    }
    static void convolution(const vector<int> &a, const vector<int> &b, vector<int> &c) {
        int sz = a.size() + b.size() - 1;
        int n = 1 << (int)ceil(log2(sz));
        vector<Cplx> av(n), bv(n), cv;
        for (int i = 0; i < (int)a.size(); i++) av[i] = a[i];
        for (int i = 0; i < (int)b.size(); i++) bv[i] = b[i];
        cv = FFT(bv);
        bv = FFT(av);
        for (int i = 0; i < n; i++) av[i] = bv[i] * cv[i];
        cv = FFT(av, -1);
        c.resize(n);
        ll carry = 0;
        for (int i = 0; i < n; i++) {
            ll t = ll(cv[i].x + 0.5) + carry;
            carry = t / FBASE;
            c[i] = t % FBASE;
        }
        for (; carry; carry /= FBASE) c.push_back(carry % FBASE); // top limb when sz is 2^k
    }
    BigInt mul_simple(const BigInt &v) const {
        BigInt res;
        res.sgn = sgn * v.sgn;
        res.a.resize(a.size() + v.a.size());
        for (int i = 0; i < (int)a.size(); i++)
            if (a[i])
                for (int j = 0, carry = 0; j < (int)v.a.size() || carry; j++) {
                    ll cur = res.a[i + j] + (ll)a[i] * (j < (int)v.a.size() ? v.a[j] : 0) + carry;
                    carry = (int)(cur / BASE);
                    res.a[i + j] = (int)(cur % BASE);
                }
        res.trim();
        return res;
    }
    BigInt mul_fft(const BigInt &v) const {
        BigInt res;
        res.sgn = sgn * v.sgn;
        convolution(convert_base(a, DIG, FDIG), convert_base(v.a, DIG, FDIG), res.a);
        res.a = convert_base(res.a, FDIG, DIG);
        res.trim();
        return res;
    }
    void operator*=(const BigInt &v) { *this = *this * v; }
    BigInt operator*(const BigInt &v) const {
        if (1LL * a.size() * v.a.size() <= 1000111) return mul_simple(v);
        return mul_fft(v);
    }

    BigInt abs() const {
        BigInt res = *this;
        res.sgn = 1;
        return res;
    }
    void trim() {
        while (!a.empty() && !a.back()) a.pop_back();
        if (a.empty()) sgn = 1; // zero is always +0, or 0 < -0 style bugs appear
    }
    bool zero() const { return a.empty() || (a.size() == 1 && !a[0]); }
    friend BigInt gcd(const BigInt &a, const BigInt &b) {
        return b.zero() ? a.abs() : gcd(b, a % b);
    }
};

BigInt power(BigInt a, ll k) { // a^k, k >= 0
    BigInt ans = 1;
    while (k > 0) {
        if (k & 1) ans *= a;
        a *= a;
        k >>= 1;
    }
    return ans;
}
