#include "../../core.h"
#include "../Primitives/Point.cpp"
#include "Sector.cpp"

template <typename T> struct Circle {
    Point<T> c; // Center
    T r;        // Radius

    Circle() {}

    Circle(Point<T> _c, T _r) : c(_c), r(_r) {}

    // Circle from 2 points (segment as diameter)
    Circle(Point<T> p1, Point<T> p2) {
        c = (p1 + p2) / 2.0;
        r = c.distance(p1);
    }

    // Circle from 3 points (Circumcircle)
    Circle(Point<T> A, Point<T> B, Point<T> C) {
        double d = 2.0 * (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));
        if (abs(d) < 1e-9) {
            c = A;
            r = 0;
            return;
        }

        double A2 = A.x * A.x + A.y * A.y;
        double B2 = B.x * B.x + B.y * B.y;
        double C2 = C.x * C.x + C.y * C.y;

        double cx = (A2 * (B.y - C.y) + B2 * (C.y - A.y) + C2 * (A.y - B.y)) / d;
        double cy = (A2 * (C.x - B.x) + B2 * (A.x - C.x) + C2 * (B.x - A.x)) / d;

        c = Point<T>(cx, cy);
        r = c.distance(A);
    }

    // Geometric Transformations
    void translate(const Point<T> &v) { c = c + v; }

    void scale(const Point<T> &center, double factor) {
        c = center + (c - center) * factor;
        r = r * factor;
    }

    void rotate(const Point<T> &center, double angle) { c = c.rotateAbout(center, angle); }

    // Point Inclusion
    bool onBoundary(const Point<T> &p) const { return abs(c.distance(p) - r) <= eps; }

    bool strictlyInside(const Point<T> &p) const { return c.distance(p) < r - eps; }

    bool contains(const Point<T> &p) const { return c.distance(p) <= r + eps; }

    // Splits the circle by an infinite line AB into two Sectors
    // Returns {Left Sector, Right Sector} with respect to the directed line AB
    pair<Sector<T>, Sector<T>> split(const Point<T> &a, const Point<T> &b) const {
        // Find line intersections
        double A = (b - a).dot(b - a);
        double B = 2 * (b - a).dot(a - c);
        double C = (a - c).dot(a - c) - r * r;

        double det = sqrt(max(0.0, B * B - 4 * A * C));
        double t1 = (-B + det) / (2 * A);
        double t2 = (-B - det) / (2 * A);

        Point<T> i1 = a + (b - a) * t1;
        Point<T> i2 = a + (b - a) * t2;

        // Ensure i1 is the first intersection along the line AB and i2 is the second
        if ((i1 - a).dot(b - a) > (i2 - a).dot(b - a)) swap(i1, i2);

        // Left side is counter-clockwise from i2 to i1
        // Right side is counter-clockwise from i1 to i2
        return {Sector<T>(c, i2, i1), Sector<T>(c, i1, i2)};
    }
};

using circle = Circle<double>;
