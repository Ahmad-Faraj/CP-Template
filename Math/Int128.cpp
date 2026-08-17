#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

/*
 * Topic: Math - 128-bit Integer Wrapper
 * Description: A struct wrapper for GCC's native `__int128_t` that provides
 * seamless std::cin and std::cout support. Because of the implicit conversion
 * operator, it inherits all +, -, *, /, %, &, |, ^ natively without needing
 * manual operator overloading.
 */
struct __int_128 {
    __int128_t val;
    
    __int_128() : val(0) {}
    __int_128(__int128_t v) : val(v) {}
    __int_128(long long v) : val(v) {}

    // Implicitly convert back to native __int128_t for math operations
    operator __int128_t() const { return val; }

    friend istream& operator>>(istream& in, __int_128& num) {
        string s;
        if (in >> s) {
            num.val = 0;
            bool neg = (s[0] == '-');
            for (int i = neg; i < (int)s.length(); ++i) {
                num.val = num.val * 10 + (s[i] - '0');
            }
            if (neg) num.val = -num.val;
        }
        return in;
    }

    friend ostream& operator<<(ostream& out, const __int_128& num) {
        if (num.val == 0) return out << "0";
        string s = "";
        __int128_t temp = num.val;
        bool neg = (temp < 0);
        if (neg) temp = -temp;
        
        while (temp > 0) {
            s += (char)('0' + (int)(temp % 10));
            temp /= 10;
        }
        if (neg) s += "-";
        
        reverse(s.begin(), s.end());
        return out << s;
    }
};
