#include "../../core.h"
#include "../Primitives/Point.cpp"

template <typename T>
struct Sector {
    Point<T> c; // Center of the pie
    Point<T> start; // Starting point of the arc
    Point<T> end; // Ending point of the arc (counter-clockwise)
    double r; // Radius
    
    Sector() {}
    Sector(Point<T> _c, Point<T> _start, Point<T> _end) : c(_c), start(_start), end(_end) {
        r = c.distance(start);
    }
    
    double getAngle() const {
        double angle1 = atan2(start.y - c.y, start.x - c.x);
        double angle2 = atan2(end.y - c.y, end.x - c.x);
        double diff = angle2 - angle1;
        if (diff < -eps) diff += 2 * acos(-1.0); // Ensure positive CCW angle
        return diff;
    }
    
    double area() const {
        return 0.5 * r * r * getAngle();
    }
    
    double perimeter() const {
        return 2 * r + r * getAngle(); // Two straight radii edges + curved arc
    }
    
    // Checks if a point lies strictly inside the pie slice
    bool strictlyInside(const Point<T>& p) const {
        if (c.distance(p) > r - eps) return false;
        double a = getAngle();
        double a1 = atan2(start.y - c.y, start.x - c.x);
        double pa = atan2(p.y - c.y, p.x - c.x);
        double diff = pa - a1;
        if (diff < -eps) diff += 2 * acos(-1.0);
        return diff < a - eps && diff > eps;
    }
};

using sector = Sector<double>;
