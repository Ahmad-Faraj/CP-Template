#include "../../core.h"

const double eps = 1e-9;

template <typename T>
struct Point {
    T x, y;
    
    // Constructors
    Point() : x(0), y(0) {}
    Point(T _x, T _y) : x(_x), y(_y) {}

    // Basic Math Operations
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    Point operator*(T c) const { return Point(x * c, y * c); }
    Point operator/(T c) const { return Point(x / c, y / c); }

    // Dot and Cross Products
    T dot(const Point& p) const { return x * p.x + y * p.y; }
    T cross(const Point& p) const { return x * p.y - y * p.x; }
    T cross(const Point& a, const Point& b) const { return (a - *this).cross(b - *this); }

    // Length and Distance
    T norm() const { return x * x + y * y; }
    double length() const { return sqrt(norm()); }
    double distance(const Point& p) const { return (*this - p).length(); }

    // Angles and Rotations
    double angle() const { return atan2(y, x); }
    Point rotate(double ang) const {
        return Point(x * cos(ang) - y * sin(ang), x * sin(ang) + y * cos(ang));
    }
    Point rotateAbout(const Point& about, double ang) const {
        return about + (*this - about).rotate(ang);
    }
    Point perp() const { return Point(-y, x); } // 90 degrees CCW

    // Comparisons
    bool operator==(const Point& p) const { return abs(x - p.x) < eps && abs(y - p.y) < eps; }
    bool operator!=(const Point& p) const { return !(*this == p); }
    bool operator<(const Point& p) const { return x < p.x - eps || (abs(x - p.x) < eps && y < p.y - eps); }
};

// Typedefs for common usages
using pt = Point<double>;
using pti = Point<long long>;

// Helper for precision comparisons
int dcmp(double a, double b) {
    return abs(a - b) <= eps ? 0 : (a < b ? -1 : 1);
}
