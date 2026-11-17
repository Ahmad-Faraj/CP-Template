#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define sz(x) (int)x.size()

// Convert Decimal to any base
string decimal_to_any_base(ll decimal, ll base) {
    if (decimal == 0) return "0";
    string num = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string result;
    do {
        result.push_back(num[decimal % base]);
        decimal /= base;
    } while (decimal != 0);
    return string(result.rbegin(), result.rend());
}

// Convert any base to decimal
ll any_base_to_decimal(string str, int base) {
    auto val = [](char c) { return (c >= '0' && c <= '9' ? (int)c - '0' : (int)c - 'A' + 10); };
    ll len = sz(str), power = 1, num = 0, i;
    for (i = len - 1; i >= 0; i--) {
        num += val(str[i]) * power;
        power = power * base;
    }
    return num;
}
