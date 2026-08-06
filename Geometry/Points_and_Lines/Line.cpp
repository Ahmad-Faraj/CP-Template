#include "../../core.h"
#include "../Primitives/Point.cpp"

template <typename T>
struct Line {
    Point<T> p, q; // A line defined by two points
    Line() {}
    Line(Point<T> _p, Point<T> _q) : p(_p), q(_q) {}
    
    // Geometric Transformations
    void translate(const Point<T>& v) {
        p = p + v;
        q = q + v;
    }
    void scale(const Point<T>& center, double factor) {
        p = center + (p - center) * factor;
        q = center + (q - center) * factor;
    }
    void rotate(const Point<T>& center, double angle) {
        p = p.rotateAbout(center, angle);
        q = q.rotateAbout(center, angle);
    }
};

using line = Line<double>;
