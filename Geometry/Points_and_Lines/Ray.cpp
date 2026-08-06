#include "../../core.h"
#include "../Primitives/Point.cpp"

template <typename T>
struct Ray {
    Point<T> origin, dir; // A ray starting at origin and pointing infinitely in dir
    
    Ray() {}
    Ray(Point<T> _origin, Point<T> _dir) : origin(_origin), dir(_dir) {}
    
    // Geometric Transformations
    void translate(const Point<T>& v) {
        origin = origin + v;
        // Direction vector remains completely unchanged under pure translation
    }
    void scale(const Point<T>& center, double factor) {
        origin = center + (origin - center) * factor;
        dir = dir * factor; // Scale the direction vector's magnitude
    }
    void rotate(const Point<T>& center, double angle) {
        origin = origin.rotateAbout(center, angle);
        dir = dir.rotate(angle); // Direction vector simply rotates in place
    }
};

using ray = Ray<double>;
