// Brute-force test for:
//   Math/Big_Integer.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

namespace BG {
#include "../Math/Big_Integer.cpp"
}

static string to_str(__int128 v) {
    if (v == 0) return "0";
    bool neg = v < 0;
    unsigned __int128 u = neg ? (unsigned __int128)(-(v + 1)) + 1 : (unsigned __int128)v;
    string s;
    while (u) {
        s += char('0' + (int)(u % 10));
        u /= 10;
    }
    if (neg) s += '-';
    reverse(s.begin(), s.end());
    return s;
}

static string show(const BG::BigInt &b) {
    ostringstream os;
    os << b;
    return os.str();
}

int main() {
    mt19937_64 gen(90210);
    int addFail = 0, subFail = 0, mulFail = 0, divFail = 0, modFail = 0;
    int cmpFail = 0, roundFail = 0, negFail = 0, zeroFail = 0, bigFail = 0, powFail = 0;
    int checked = 0;

    // ---------- against __int128, for values that fit ----------
    for (int it = 0; it < 60000; it++) {
        // mix magnitudes: small, medium and near the 64-bit edge
        int scale = it % 3;
        auto pick = [&]() -> __int128 {
            ll v;
            if (scale == 0) v = (ll)(gen() % 41) - 20;
            else if (scale == 1) v = (ll)(gen() % 2000001) - 1000000;
            else v = (ll)(gen() % 4000000000000000000ULL) - 2000000000000000000LL;
            return (__int128)v;
        };
        __int128 x = pick(), y = pick();
        BG::BigInt a(to_str(x)), b(to_str(y));
        checked++;

        // round trip through the string form
        if (show(a) != to_str(x)) roundFail++;
        if (show(b) != to_str(y)) roundFail++;

        if (show(a + b) != to_str(x + y)) addFail++;
        if (show(a - b) != to_str(x - y)) subFail++;
        if (show(a * b) != to_str(x * y)) mulFail++;
        if (show(-a) != to_str(-x)) negFail++;

        if (y != 0) {
            // C++ truncates toward zero for both / and %, and BigInt must match
            if (show(a / b) != to_str(x / y)) divFail++;
            if (show(a % b) != to_str(x % y)) modFail++;
        }

        // comparisons must agree exactly
        if ((a < b) != (x < y)) cmpFail++;
        if ((a > b) != (x > y)) cmpFail++;
        if ((a == b) != (x == y)) cmpFail++;
        if ((a != b) != (x != y)) cmpFail++;
        if ((a <= b) != (x <= y)) cmpFail++;
        if ((a >= b) != (x >= y)) cmpFail++;

        // compound assignment must match the binary form
        BG::BigInt c = a;
        c += b;
        if (show(c) != to_str(x + y)) addFail++;
        c = a;
        c -= b;
        if (show(c) != to_str(x - y)) subFail++;
    }

    // ---------- zero and sign edge cases ----------
    {
        BG::BigInt z(0), one(1), negone(-1);
        if (show(z) != "0") zeroFail++;
        if (show(z + z) != "0") zeroFail++;
        if (show(z - z) != "0") zeroFail++;
        if (show(z * one) != "0") zeroFail++;
        if (show(-z) != "0") zeroFail++;                 // negative zero must print as 0
        if (show(one - one) != "0") zeroFail++;
        if (show(one + negone) != "0") zeroFail++;
        if (!(z == BG::BigInt(0))) zeroFail++;
        if (show(z / one) != "0") zeroFail++;
        if (show(z % one) != "0") zeroFail++;
        // leading zeros on input must not survive
        if (show(BG::BigInt(string("000123"))) != "123") roundFail++;
        if (show(BG::BigInt(string("-000123"))) != "-123") roundFail++;
        if (show(BG::BigInt(string("0000"))) != "0") roundFail++;
    }

    // ---------- values far beyond 64 bits, against constants computed independently ----------
    {
        // 50!
        BG::BigInt f(1);
        for (int i = 2; i <= 50; i++) f *= BG::BigInt(i);
        if (show(f) != "30414093201713378043612608166064768844377641568960512000000000000") bigFail++;

        // 2^200 and 3^100 via the file's own power()
        if (show(BG::power(BG::BigInt(2), 200)) !=
            "1606938044258990275541962092341162602522202993782792835301376") powFail++;
        if (show(BG::power(BG::BigInt(3), 100)) != "515377520732011331036461129765621272702107522001") powFail++;

        // 10^60 + 7 against 10^30 + 3
        BG::BigInt A(string("1000000000000000000000000000000000000000000000000000000000007"));
        BG::BigInt B(string("1000000000000000000000000000003"));
        if (show(A / B) != "999999999999999999999999999997") bigFail++;
        if (show(A % B) != "16") bigFail++;
        if (show(A * B) != "10000000000000000000000000000030000000000000000000000000000070000"
                           "00000000000000000000000021") bigFail++;

        // the division identity must hold exactly: a == (a / b) * b + a % b
        if (show((A / B) * B + (A % B)) != show(A)) divFail++;

        // a long multiplication where FFT rounding would show up: 500 nines squared
        string nines(500, '9');
        BG::BigInt N(nines);
        BG::BigInt sq = N * N;
        // (10^500 - 1)^2 = 10^1000 - 2*10^500 + 1, i.e. 499 nines, an 8, 500 zeros, a 1
        string want = string(499, '9') + "8" + string(499, '0') + "1";
        if (show(sq) != want) bigFail++;

        // and that the same value divides back cleanly
        if (show(sq / N) != nines) divFail++;
        if (show(sq % N) != "0") bigFail++;
    }

    // ---------- the division identity over many random pairs ----------
    for (int it = 0; it < 5000; it++) {
        ll x = (ll)(gen() % 2000000001ULL) - 1000000000LL;
        ll y = (ll)(gen() % 2000001ULL) - 1000000LL;
        if (y == 0) continue;
        BG::BigInt a(to_str(x)), b(to_str(y));
        BG::BigInt q = a / b, r = a % b;
        if (show(q * b + r) != to_str((__int128)x)) divFail++;
        // the remainder must have the dividend's sign, matching C++
        __int128 wantR = (__int128)x % y;
        if (show(r) != to_str(wantR)) modFail++;
    }

    cout << "Big_Integer: " << checked << " random pairs vs __int128\n";
    cout << "  string round trip              : " << roundFail << " failures\n";
    cout << "  addition                       : " << addFail << " failures\n";
    cout << "  subtraction                    : " << subFail << " failures\n";
    cout << "  multiplication                 : " << mulFail << " failures\n";
    cout << "  division (truncating to zero)  : " << divFail << " failures\n";
    cout << "  remainder (sign of dividend)   : " << modFail << " failures\n";
    cout << "  comparisons                    : " << cmpFail << " failures\n";
    cout << "  negation                       : " << negFail << " failures\n";
    cout << "  zero and leading-zero handling : " << zeroFail << " failures\n";
    cout << "  values beyond 64 bits          : " << bigFail << " failures\n";
    cout << "  power()                        : " << powFail << " failures\n";
}
