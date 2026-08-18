// Expression Parsing: evaluates an infix arithmetic expression, or converts it to postfix.
// Use when: the input IS a formula - calculator problems, expressions with nested brackets, formula rewriting.
// Handles: + - * / %, ^ (right associative), unary minus and plus, nested brackets, spaces, multi-digit numbers.
// Time: O(len) to evaluate, single pass with two stacks
// Indexing: 0-based over the input string
// Note: integer division truncates toward zero, as C++ does. Division or modulo by zero will trap - guard the input.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Expression_Parsing {
    static int precedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/' || op == '%') return 2;
        if (op == '^') return 4;
        return 0;
    }

    static bool right_associative(char op) { return op == '^'; } // 2^3^2 is 2^(3^2)

    // 'u' is unary minus, 'p' unary plus. They bind tighter than * and /, but LOOSER than ^,
    // so -2^2 is -(2^2) = -4, which is the usual reading. Rank 3 sits between the two.
    static bool unary(char op) { return op == 'u' || op == 'p'; }

    static ll apply(char op, ll a, ll b) {
        switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '%': return a % b;
        case '^': {
            ll r = 1;
            while (b > 0) {
                if (b & 1) r *= a;
                a *= a;
                b >>= 1;
            }
            return r;
        }
        }
        return 0;
    }

    static void collapse(vector<ll> &values, vector<char> &ops) {
        char op = ops.back();
        ops.pop_back();
        if (unary(op)) {
            ll v = values.back();
            values.pop_back();
            values.push_back(op == 'u' ? -v : v);
        } else {
            ll b = values.back();
            values.pop_back();
            ll a = values.back();
            values.pop_back();
            values.push_back(apply(op, a, b));
        }
    }

    static ll evaluate(const string &s) {
        vector<ll> values;
        vector<char> ops;
        bool expect_operand = true; // so a '-' here is unary, not subtraction
        for (size_t i = 0; i < s.size(); i++) {
            char c = s[i];
            if (isspace((unsigned char)c)) continue;
            if (isdigit((unsigned char)c)) {
                ll num = 0;
                while (i < s.size() && isdigit((unsigned char)s[i])) num = num * 10 + (s[i++] - '0');
                i--;
                values.push_back(num);
                expect_operand = false;
            } else if (c == '(') {
                ops.push_back('(');
                expect_operand = true;
            } else if (c == ')') {
                while (!ops.empty() && ops.back() != '(') collapse(values, ops);
                if (!ops.empty()) ops.pop_back();
                expect_operand = false;
            } else {
                char op = c;
                if (expect_operand && (c == '-' || c == '+')) op = (c == '-' ? 'u' : 'p');
                // a prefix operator never pops: its operand has not been read yet
                while (!unary(op) && !ops.empty() && ops.back() != '(') {
                    char top = ops.back();
                    int pt = unary(top) ? 3 : precedence(top);
                    int pc = precedence(op);
                    if (pt > pc || (pt == pc && !right_associative(op))) collapse(values, ops);
                    else break;
                }
                ops.push_back(op);
                expect_operand = true;
            }
        }
        while (!ops.empty()) collapse(values, ops);
        return values.empty() ? 0 : values.back();
    }

    // the same expression in postfix (reverse Polish), tokens separated by spaces
    static string to_postfix(const string &s) {
        string out;
        vector<char> ops;
        bool expect_operand = true;
        auto emit = [&](const string &tok) {
            if (!out.empty()) out += ' ';
            out += tok;
        };
        for (size_t i = 0; i < s.size(); i++) {
            char c = s[i];
            if (isspace((unsigned char)c)) continue;
            if (isdigit((unsigned char)c) || isalpha((unsigned char)c)) {
                string tok;
                while (i < s.size() && (isdigit((unsigned char)s[i]) || isalpha((unsigned char)s[i]))) tok += s[i++];
                i--;
                emit(tok);
                expect_operand = false;
            } else if (c == '(') {
                ops.push_back('(');
                expect_operand = true;
            } else if (c == ')') {
                while (!ops.empty() && ops.back() != '(') emit(string(1, ops.back())), ops.pop_back();
                if (!ops.empty()) ops.pop_back();
                expect_operand = false;
            } else {
                char op = c;
                if (expect_operand && (c == '-' || c == '+')) op = (c == '-' ? 'u' : 'p');
                while (!unary(op) && !ops.empty() && ops.back() != '(') {
                    char top = ops.back();
                    int pt = unary(top) ? 3 : precedence(top);
                    int pc = precedence(op);
                    if (pt > pc || (pt == pc && !right_associative(op))) emit(string(1, ops.back())), ops.pop_back();
                    else break;
                }
                ops.push_back(op);
                expect_operand = true;
            }
        }
        while (!ops.empty()) emit(string(1, ops.back())), ops.pop_back();
        return out;
    }
};

// Standard problem: one infix expression per line - print its value
void solve() {
    string line;
    while (getline(cin, line)) {
        if (line.find_first_not_of(" \t\r\n") == string::npos) continue;
        cout << Expression_Parsing::evaluate(line) << '\n';
    }
}
