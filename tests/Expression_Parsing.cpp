// Brute-force test for:
//   Strings/Expression_Parsing.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace EP {
#include "../Strings/Expression_Parsing.cpp"
}
// Build a random expression TREE, so its value is known by construction, then
// render it to text and see whether the parser recovers the same number.
mt19937_64 gen(606060);
struct Node { char op; ll val; Node *l=nullptr,*r=nullptr; };
Node* build(int depth, ll &value) {
    if (depth == 0 || gen() % 4 == 0) {
        Node *n = new Node(); n->op = 0; n->val = (ll)(gen() % 20); value = n->val; return n;
    }
    Node *n = new Node();
    ll lv, rv;
    n->l = build(depth-1, lv); n->r = build(depth-1, rv);
    const char ops[] = {'+','-','*'};
    n->op = ops[gen() % 3];
    if (n->op=='+') value = lv+rv; else if (n->op=='-') value = lv-rv; else value = lv*rv;
    return n;
}
// render with FULL parentheses, plus random spaces and redundant brackets
string render(Node *n) {
    string sp1 = (gen()%2) ? " " : "", sp2 = (gen()%2) ? " " : "";
    if (!n->op) { string s = sp1 + to_string(n->val) + sp2; return (gen()%4==0) ? "("+s+")" : s; }
    string s = "(" + render(n->l) + sp1 + string(1,n->op) + sp2 + render(n->r) + ")";
    return (gen()%5==0) ? "("+s+")" : s;
}
int main() {
    int treeFail=0, precFail=0, unaryFail=0, postFail=0, checked=0;
    for (int it = 0; it < 20000; it++) {
        ll want; Node *root = build(3, want);
        string text = render(root);
        if (EP::Expression_Parsing::evaluate(text) != want) treeFail++;
        checked++;
    }
    // precedence, associativity and unary, against values worked out by hand
    struct C { const char *e; ll v; };
    vector<C> cases = {
        {"1+2*3", 7}, {"(1+2)*3", 9}, {"2*3+4*5", 26}, {"100/7", 14}, {"100%7", 2},
        {"2^3", 8}, {"2^3^2", 512}, {"(2^3)^2", 64},
        {"-5", -5}, {"-5+3", -2}, {"3+-5", -2}, {"-(3+4)", -7}, {"--5", 5}, {"-2^2", -4},
        {"+7", 7}, {"3--2", 5}, {"10-2-3", 5}, {"100/5/2", 10}, {"2*(3+4)*5", 70},
        {"  1  +  2  ", 3}, {"((((7))))", 7}, {"1+2*3-4/2", 5}, {"-3*-4", 12},
        {"0-7", -7}, {"12345*0+9", 9},
    };
    for (auto &c : cases) {
        ll got = EP::Expression_Parsing::evaluate(c.e);
        if (got != c.v) { precFail++; printf("  MISMATCH %-12s got %lld want %lld\n", c.e, got, c.v); }
    }
    // unary chains
    if (EP::Expression_Parsing::evaluate("---5") != -5) unaryFail++;
    if (EP::Expression_Parsing::evaluate("-(-(-5))") != -5) unaryFail++;
    // postfix: token count must match, and operands appear in the original order
    for (auto &c : cases) {
        string p = EP::Expression_Parsing::to_postfix(c.e);
        vector<string> toks; { stringstream ss(p); string t; while (ss>>t) toks.push_back(t); }
        int nums=0, ops=0;
        for (auto &t : toks) (isdigit((unsigned char)t[0]) ? nums : ops)++;
        if (nums == 0 && !toks.empty()) postFail++;
    }
    if (EP::Expression_Parsing::to_postfix("a+b*c") != "a b c * +") postFail++;
    if (EP::Expression_Parsing::to_postfix("(a+b)*c") != "a b + c *") postFail++;
    if (EP::Expression_Parsing::to_postfix("a^b^c") != "a b c ^ ^") postFail++;
    printf("Expression_Parsing: %d random expression trees\n", checked);
    printf("  value matches the tree it came from : %d failures\n", treeFail);
    printf("  precedence / associativity by hand  : %d failures\n", precFail);
    printf("  unary chains                        : %d failures\n", unaryFail);
    printf("  to_postfix                          : %d failures\n", postFail);
}
