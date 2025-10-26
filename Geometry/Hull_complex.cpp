#include <bits/stdc++.h>
using namespace std;

#define pt complex<double>
#define ld long double

const double eps = 1e-12;

int dcmp(ld a, ld b) {
    return fabs(a - b) <= eps ? 0 : a < b ? -1 : 1;
}

ld dot(pt a, pt b) {
    // return a.X * b.X + a.Y * b.Y​
    return ((conj(a) * (b)).real());
}

ld cross(pt a, pt b) {
    // return a.X * b.Y - b.X * a.Y​
    return ((conj(a) * (b)).imag());
}

bool isCollinear(pt a, pt b, pt c) {
    return dcmp(cross(b - a, c - a), 0) == 0;
}

pt pivot;
bool sortCCW(const pt &a, const pt &b) {
    if (dcmp(cross(a - pivot, b - pivot), 0) == 0) {
        if (dcmp(a.imag(), b.imag()) != 0) {
            return a.imag() < b.imag();
        }
        return a.real() < b.real();
    }
    return cross(a - pivot, b - pivot) >= eps;
}

bool cmp(const pt &a, const pt &b) {
    if (dcmp(a.real(), b.real()) != 0) return a.real() < b.real();
    return a.imag() < b.imag();
}

vector<pt> convexHull(vector<pt> &v) {
    sort(v.begin(), v.end(), cmp);
    /// remove duplicate points
    vector<pt> p;
    p.push_back(v[0]);
    for (int i = 1; i < v.size(); i++) {
        if (v[i] == v[i - 1]) continue;
        p.push_back(v[i]);
    }
    if (p.size() <= 1) return p;
    v = p;
    for (int i = 1; i < (int)v.size(); i++) {
        if (v[i].imag() < v[0].imag())
            swap(v[i], v[0]);
        else if (dcmp(v[i].imag(), v[0].imag()) == 0 && v[i].real() < v[0].real())
            swap(v[i], v[0]);
    }
    pivot = v[0];
    sort(v.begin() + 1, v.end(), sortCCW);
    // ^^^ removes duplicate and sorts CCW according to bottom left point
    vector<pt> ch;
    v.push_back(v[0]);
    for (int i = 0; i < v.size(); i++) {
        while (ch.size() > 1) {
            auto cur = ch.back(), prv = ch[ch.size() - 2];
            if (cross(prv - cur, v[i] - cur) <= 0) break;
            ch.pop_back();
        }
        ch.push_back(v[i]);
    }
    v.pop_back();
    ch.pop_back();
    /// ch is convex hull with collinear v
    // return ch;
    /// removing collinear v
    vector<pt> finalCH = {ch[0]};
    for (int i = 1; i + 1 < ch.size(); i++)
        if (!isCollinear(ch[i - 1], ch[i], ch[i + 1])) finalCH.push_back(ch[i]);
    finalCH.push_back(ch.back());
    return finalCH;
}
