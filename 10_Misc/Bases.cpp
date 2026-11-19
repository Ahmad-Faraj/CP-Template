// Bases: convert a number between base 10 and any base 2..36 (digits 0-9 then A-Z).
// Use when: "write n in base b", read a number given in base b, base-k digit tricks.
// Usage: string s = decimal_to_any_base(n, b); ll n = any_base_to_decimal(s, b);
// Handles: 0, negative numbers (leading '-'), the full ll range including LLONG_MIN,
//          lower- or upper-case digits on input.
// Time: O(number of digits)
// Indexing: none
// Note: the input string must be a valid number in that base whose value fits in ll.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

string decimal_to_any_base(ll decimal, ll base) {
    if (decimal == 0) return "0";
    string num = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned long long x = decimal < 0 ? 0ULL - (unsigned long long)decimal : decimal;
    string result;
    do {
        result.push_back(num[x % base]);
        x /= base;
    } while (x != 0);
    if (decimal < 0) result.push_back('-');
    return string(result.rbegin(), result.rend());
}

ll any_base_to_decimal(const string &str, ll base) {
    auto val = [](char c) { return isdigit(c) ? c - '0' : toupper(c) - 'A' + 10; };
    bool neg = !str.empty() && str[0] == '-';
    unsigned long long num = 0;
    for (int i = neg; i < (int)str.size(); i++) num = num * base + val(str[i]);
    return neg ? (ll)(0ULL - num) : (ll)num;
}
