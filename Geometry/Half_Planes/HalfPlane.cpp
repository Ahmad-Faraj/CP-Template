#include "../../core.h"
#include "../Primitives/Point.cpp"

// Represents a Half-Plane defined by a line passing through 'p' in direction 'dir'.
// The valid region is considered to be strictly on the LEFT side of the directed line.
template <typename T> struct HalfPlane {
    Point<T> p, dir;
    double angle;

    HalfPlane() {}

    HalfPlane(Point<T> a, Point<T> b) : p(a), dir(b - a) { angle = atan2(dir.y, dir.x); }

    // Checks if a point 'q' is strictly outside this half-plane (i.e. on the right side)
    bool out(const Point<T> &q) const { return dir.cross(q - p) < -eps; }

    // Sorting comparator for Half-Planes (by angle)
    bool operator<(const HalfPlane &other) const { return angle < other.angle; }
};

using hp = HalfPlane<double>;
