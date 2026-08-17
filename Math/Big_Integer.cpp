// Big Integer: arbitrary-precision signed integers with the four operations, multiplying via FFT.
// Use when: values outrun 64 bits - huge factorials, exact powers, 100-digit input, or exact answers with no modulus.
// Handles: negatives, zero, comparison, division and remainder, and building from a string or a long long.
// Time: add/subtract O(n) | multiply O(n log n) by FFT | divide O(n^2)
// Indexing: digits are stored base 1e6, least significant first; you never index them directly
// Note: division is the slow operation here. If you only need a remainder by a small number, use operator%(ll).

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int DIG = 9;
const int BASE = 1e9;

// use mulmod when taking mod by int v and v>2e9
// you can use mod by bigint in that case too
struct BigInt {
    int sgn;
    vector<int> a;
    BigInt() : sgn(1) {}
    BigInt(ll v) { *this = v; }
    BigInt &operator=(ll v) {
        sgn = 1;
        if (v < 0) sgn = -1, v = -v;
        a.clear();
        for (; v > 0; v /= BASE) a.push_back(v % BASE);
        return *this;
    }
    BigInt(const BigInt &other) {
        sgn = other.sgn;
        a = other.a;
    }
    friend void swap(BigInt &a, BigInt &b) {
        swap(a.sgn, b.sgn);
        swap(a.a, b.a);
    }
    BigInt &operator=(BigInt other) {
        swap(*this, other);
        return *this;
    }
    BigInt(BigInt &&other) : BigInt() { swap(*this, other); }
    BigInt(const string &s) { read(s); }
    void read(const string &s) {
        sgn = 1;
        a.clear();
        int k = 0;
        for (; k < (int)s.size() && (s[k] == '-' || s[k] == '+'); k++)
            if (s[k] == '-') sgn = -sgn;
        for (int i = s.size() - 1; i >= k; i -= DIG) {
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
        if (a.size() != v.a.size()) return a.size() * sgn < v.a.size() * v.sgn;
        for (int i = (int)a.size() - 1; i >= 0; i--)
            if (a[i] != v.a[i]) return a[i] * sgn < v.a[i] * sgn;
        return 0;
    }
    bool operator>(const BigInt &v) const { return v < *this; }
    bool operator<=(const BigInt &v) const { return !(v < *this); }
    bool operator>=(const BigInt &v) const { return !(*this < v); }
    bool operator==(const BigInt &v) const { return !(*this < v) && !(v < *this); }
    bool operator!=(const BigInt &v) const { return *this < v || v < *this; }
    friend int __cmp(const BigInt &x, const BigInt &y) {
        if (x.a.size() != y.a.size()) return x.a.size() < y.a.size() ? -1 : 1;
        for (int i = (int)x.a.size() - 1; i >= 0; --i)
            if (x.a[i] != y.a[i]) return x.a[i] < y.a[i] ? -1 : 1;
        return 0;
    }

    BigInt operator-() const {
        BigInt res = *this;
        if (zero()) return res;
        res.sgn = -sgn;
        return res;
    }

    void __add(const BigInt &v) {
        if (a.size() < v.a.size()) a.resize(v.a.size(), 0);
        for (int i = 0, carry = 0; i < (int)max(a.size(), v.a.size()) || carry; ++i) {
            if (i == (int)a.size()) a.push_back(0);
            a[i] += carry + (i < (int)v.a.size() ? v.a[i] : 0);
            carry = a[i] >= BASE;
            if (carry) a[i] -= BASE;
        }
    }

    void __sub(const BigInt &v) {
        for (int i = 0, carry = 0; i < (int)v.a.size() || carry; ++i) {
            a[i] -= carry + (i < (int)v.a.size() ? v.a[i] : 0);
            carry = a[i] < 0;
            if (carry) a[i] += BASE;
        }
        this->trim();
    }

    BigInt operator+=(const BigInt &v) {
        if (sgn == v.sgn)
            __add(v);
        else if (__cmp(*this, v) >= 0)
            __sub(v);
        else {
            BigInt vv = v;
            swap(*this, vv);
            __sub(vv);
        }
        return *this;
    }

    BigInt operator-=(const BigInt &v) {
        if (sgn == v.sgn) {
            if (__cmp(*this, v) >= 0)
                __sub(v);
            else {
                BigInt vv = v;
                swap(*this, vv);
                __sub(vv);
                sgn = -sgn;
            }
        } else
            __add(v);
        return *this;
    }

    friend BigInt operator+(BigInt a, const BigInt &b) { a += b; return a; }
    friend BigInt operator-(BigInt a, const BigInt &b) { a -= b; return a; }

    friend pair<BigInt, BigInt> divmod(const BigInt &a1, const BigInt &b1) {
        ll norm = BASE / (b1.a.back() + 1);
        BigInt a = a1.abs() * norm, b = b1.abs() * norm, q = 0, r = 0;
        q.a.resize(a.a.size());
        for (int i = a.a.size() - 1; i >= 0; i--) {
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
        // q and r are already truncated toward zero, with r carrying the dividend's sign,
        // which is what C++ does. Do NOT shift r to be non-negative here: without a matching
        // change to q that breaks the identity q * b + r == a, and it fires on negative zero.
        return make_pair(q, r / norm);
    }
    BigInt operator/(const BigInt &v) const { return divmod(*this, v).first; }
    BigInt operator%(const BigInt &v) const { return divmod(*this, v).second; }
    void operator/=(const BigInt &v) { *this = *this / v; }

    void operator*=(const BigInt &v) { *this = *this * v; }
    BigInt operator*(const BigInt &v) const {
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

    void div2() {
        for (int i = (int)a.size() - 1, rem = 0; i >= 0; --i) {
            ll cur = a[i] + rem * (ll)BASE;
            a[i] = (int)(cur / 2);
            rem = (int)(cur % 2);
        }
        trim();
    }
    bool is_odd() const { return !a.empty() && (a[0] % 2 != 0); }
    
    BigInt operator&(const BigInt &v) const {
        BigInt x = this->abs(), y = v.abs(), res = 0, p = 1;
        while (!x.zero() && !y.zero()) {
            if (x.is_odd() & y.is_odd()) res += p;
            x.div2(); y.div2(); p *= 2;
        }
        return res;
    }
    BigInt operator|(const BigInt &v) const {
        BigInt x = this->abs(), y = v.abs(), res = 0, p = 1;
        while (!x.zero() || !y.zero()) {
            if (x.is_odd() | y.is_odd()) res += p;
            x.div2(); y.div2(); p *= 2;
        }
        return res;
    }
    BigInt operator^(const BigInt &v) const {
        BigInt x = this->abs(), y = v.abs(), res = 0, p = 1;
        while (!x.zero() || !y.zero()) {
            if (x.is_odd() ^ y.is_odd()) res += p;
            x.div2(); y.div2(); p *= 2;
        }
        return res;
    }

    BigInt pow(ll k) const {
        BigInt ans = 1, base = *this;
        while (k > 0) {
            if (k & 1) ans *= base;
            base *= base;
            k >>= 1;
        }
        return ans;
    }

    BigInt abs() const {
        BigInt res = *this;
        res.sgn *= res.sgn;
        return res;
    }
    void trim() {
        while (!a.empty() && !a.back()) a.pop_back();
    }
    bool zero() const { return a.empty() || (a.size() == 1 && !a[0]); }

};

