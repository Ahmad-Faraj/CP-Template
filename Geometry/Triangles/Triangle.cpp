#include "../../core.h"
#include "../Primitives/Point.cpp"

template <typename T> struct Triangle {
    Point<T> a, b, c; // The three vertices of the triangle

    Triangle() {}

    Triangle(Point<T> _a, Point<T> _b, Point<T> _c) : a(_a), b(_b), c(_c) {}

    // Equilateral Triangle from Center and a Vertex
    static Triangle<T> createEquilateralCenter(Point<T> center, Point<T> vertex) {
        double angle = 2.0 * acos(-1.0) / 3.0; // 120 degrees
        return Triangle<T>(vertex, vertex.rotateAbout(center, angle), vertex.rotateAbout(center, 2 * angle));
    }

    // Equilateral Triangle from an Edge (p1, p2)
    static Triangle<T> createEquilateralEdge(Point<T> p1, Point<T> p2) {
        double angle = acos(-1.0) / 3.0; // 60 degrees
        return Triangle<T>(p1, p2, p2.rotateAbout(p1, angle));
    }

    // SAS (Side-Angle-Side)
    static Triangle<T> createSAS(Point<T> A, Point<T> B, double angle_A, double len_AC) {
        Point<T> dir = (B - A);
        dir = dir / dir.length();
        Point<T> C = A + dir.rotate(angle_A) * len_AC;
        return Triangle<T>(A, B, C);
    }

    // ASA (Angle-Side-Angle)
    static Triangle<T> createASA(Point<T> A, Point<T> B, double angle_A, double angle_B) {
        Point<T> dirA = (B - A).rotate(angle_A);
        Point<T> dirB = (A - B).rotate(-angle_B);
        double cross = dirA.cross(dirB);
        Point<T> C = A;
        if (abs(cross) > eps) {
            double t = (B - A).cross(dirB) / cross;
            C = A + dirA * t;
        }
        return Triangle<T>(A, B, C);
    }

    // SSS (Side-Side-Side)
    static Triangle<T> createSSS(Point<T> A, Point<T> B, double len_AC, double len_BC) {
        double len_AB = A.distance(B);
        double cosA = (len_AC * len_AC + len_AB * len_AB - len_BC * len_BC) / (2 * len_AC * len_AB);
        cosA = max(-1.0, min(1.0, cosA));
        double angle_A = acos(cosA);
        Point<T> dir = (B - A);
        dir = dir / dir.length();
        Point<T> C = A + dir.rotate(angle_A) * len_AC;
        return Triangle<T>(A, B, C);
    }

    // Geometric Transformations
    void translate(const Point<T> &v) {
        a = a + v;
        b = b + v;
        c = c + v;
    }

    void scale(const Point<T> &center, double factor) {
        a = center + (a - center) * factor;
        b = center + (b - center) * factor;
        c = center + (c - center) * factor;
    }

    void rotate(const Point<T> &center, double angle) {
        a = a.rotateAbout(center, angle);
        b = b.rotateAbout(center, angle);
        c = c.rotateAbout(center, angle);
    }

    // Measurements
    double perimeter() const { return a.distance(b) + b.distance(c) + c.distance(a); }

    double area() const { return abs((b - a).cross(c - a)) / 2.0; }

    // Point Inclusion
    bool onBoundary(const Point<T> &p) const {
        auto onSeg = [&](Point<T> p1, Point<T> p2) {
            return abs((p2 - p1).cross(p - p1)) < eps && (p - p1).dot(p - p2) <= eps;
        };
        return onSeg(a, b) || onSeg(b, c) || onSeg(c, a);
    }

    bool strictlyInside(const Point<T> &p) const {
        double cross1 = (b - a).cross(p - a);
        double cross2 = (c - b).cross(p - b);
        double cross3 = (a - c).cross(p - c);
        return (cross1 > eps && cross2 > eps && cross3 > eps) || (cross1 < -eps && cross2 < -eps && cross3 < -eps);
    }

    bool contains(const Point<T> &p) const { return strictlyInside(p) || onBoundary(p); }
};

using triangle = Triangle<double>;
