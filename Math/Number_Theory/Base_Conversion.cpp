// Base Conversion: an integer to a string in any base from 2 to 36, and back again.
// Use when: "write n in base b", reading a hexadecimal or base-36 token, digit-sum problems in an unusual base.
// Handles: zero, negative values (a leading minus), lower or upper case digits on input, and bases 2 through 36.
// Time: O(log_b n) each way
// Indexing: digits are most-significant first, as written
// Note: to_base gives "0" for zero, not an empty string. from_base stops at the first character that is not a digit.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const string DIGITS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

string to_base(ll value, int base) { // base 2..36
    if (value == 0) return "0";
    bool negative = value < 0;
    unsigned long long v = negative ? (unsigned long long)(-(value + 1)) + 1 : (unsigned long long)value;
    string out;
    while (v) {
        out.push_back(DIGITS[v % base]);
        v /= base;
    }
    if (negative) out.push_back('-');
    reverse(out.begin(), out.end());
    return out;
}

int digit_value(char c) { // -1 when c is not a digit in any base
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if (c >= 'a' && c <= 'z') return c - 'a' + 10;
    return -1;
}

ll from_base(const string &s, int base) { // accepts either case; a leading '-' negates
    ll result = 0;
    size_t i = 0;
    bool negative = false;
    if (i < s.size() && (s[i] == '-' || s[i] == '+')) negative = (s[i] == '-'), i++;
    for (; i < s.size(); i++) {
        int d = digit_value(s[i]);
        if (d < 0 || d >= base) break; // stop at the first character that is not a digit of this base
        result = result * base + d;
    }
    return negative ? -result : result;
}

bool valid_in_base(const string &s, int base) { // every character is a digit of this base
    size_t i = (!s.empty() && (s[0] == '-' || s[0] == '+')) ? 1 : 0;
    if (i >= s.size()) return false;
    for (; i < s.size(); i++) {
        int d = digit_value(s[i]);
        if (d < 0 || d >= base) return false;
    }
    return true;
}

// Standard problem: a number written in base from, re-written in base to
void solve() {
    string s;
    int from, to;
    cin >> s >> from >> to;
    cout << to_base(from_base(s, from), to) << '\n';
}
