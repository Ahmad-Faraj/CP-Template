#include <bits/stdc++.h>
using namespace std;

#define ld long double
#define pt complex<double>

const double PI = acos(-1);
const double eps = 1e-12;

double length(pt p1, pt p2) {
    double dx = p1.real() - p2.real();
    double dy = p1.imag() - p2.imag();
    return sqrt(dx * dx + dy * dy);
}

double triangle_area(pt p0, pt p1, pt p2) {
    double a = length(p0, p1), b = length(p0, p2), c = length(p1, p2);
    double s = (a + b + c) / 2;
    return sqrt((s - a) * (s - b) * (s - c) * s);
}

double triangle_area_medians(double m1, double m2, double m3) {
    if (m1 <= 0 || m2 <= 0 || m3 <= 0) return -1;
    double s = (m1 + m2 + m3) / 2;
    double medians_area = s * (s - m1) * (s - m2) * (s - m3);
    double area = (4.0 / 3.0) * sqrt(medians_area);
    if (medians_area <= 0 || area <= 0) return -1;
    return area;
}
