#include "../../core.h"
#include "../Primitives/Point.cpp"

template <typename T>
struct Polygon {
    vector<Point<T>> pts;
    Polygon() {}
    Polygon(const vector<Point<T>>& _pts) : pts(_pts) {}
    
    // Regular Polygon from Center and a Vertex
    static Polygon<T> createRegularCenter(Point<T> center, Point<T> vertex, int n) {
        Polygon<T> p;
        double angle = 2.0 * acos(-1.0) / n;
        for (int i = 0; i < n; i++) {
            p.pts.push_back(vertex.rotateAbout(center, angle * i));
        }
        return p;
    }

    // Regular Polygon given two adjacent vertices (in CCW order)
    static Polygon<T> createRegularEdge(Point<T> p1, Point<T> p2, int n) {
        Polygon<T> p;
        p.pts.push_back(p1);
        p.pts.push_back(p2);
        double ext_angle = 2.0 * acos(-1.0) / n;
        Point<T> dir = p2 - p1;
        Point<T> last = p2;
        for (int i = 2; i < n; i++) {
            dir = dir.rotate(ext_angle);
            last = last + dir;
            p.pts.push_back(last);
        }
        return p;
    }
    
    int size() const { return pts.size(); }
    void add_point(const Point<T>& p) { pts.push_back(p); }
    Point<T>& operator[](int i) { return pts[i]; }
    const Point<T>& operator[](int i) const { return pts[i]; }
    
    // Geometric Transformations
    void translate(const Point<T>& v) {
        for (auto& p : pts) p = p + v;
    }
    void scale(const Point<T>& center, double factor) {
        for (auto& p : pts) p = center + (p - center) * factor;
    }
    void rotate(const Point<T>& center, double angle) {
        for (auto& p : pts) p = p.rotateAbout(center, angle);
    }

    // Measurements
    double perimeter() const {
        double res = 0;
        for (int i = 0; i < size(); i++) {
            res += pts[i].distance(pts[(i + 1) % size()]);
        }
        return res;
    }

    double area() const {
        double res = 0;
        for (int i = 0; i < size(); i++) {
            res += pts[i].cross(pts[(i + 1) % size()]);
        }
        return abs(res) / 2.0;
    }

    // Point Inclusion
    bool onBoundary(const Point<T>& p) const {
        for (int i = 0; i < size(); i++) {
            Point<T> a = pts[i], b = pts[(i + 1) % size()];
            if (abs((b - a).cross(p - a)) < eps && (p - a).dot(p - b) <= eps) return true;
        }
        return false;
    }
    
    bool contains(const Point<T>& p) const {
        if (onBoundary(p)) return true;
        bool in = false;
        for (int i = 0, j = size() - 1; i < size(); j = i++) {
            if ((pts[i].y > p.y) != (pts[j].y > p.y) &&
                (p.x < (pts[j].x - pts[i].x) * (p.y - pts[i].y) / (pts[j].y - pts[i].y) + pts[i].x))
                in = !in;
        }
        return in;
    }
    
    bool strictlyInside(const Point<T>& p) const {
        return contains(p) && !onBoundary(p);
    }

    // Center of Mass (Centroid)
    Point<double> centroid() const {
        double A = 0, cx = 0, cy = 0;
        for (int i = 0; i < size(); i++) {
            double cross = pts[i].cross(pts[(i + 1) % size()]);
            A += cross;
            cx += (pts[i].x + pts[(i + 1) % size()].x) * cross;
            cy += (pts[i].y + pts[(i + 1) % size()].y) * cross;
        }
        A *= 0.5;
        return Point<double>(cx / (6.0 * A), cy / (6.0 * A));
    }

    // Splits a convex polygon by a directed line AB into {Left Polygon, Right Polygon}
    pair<Polygon<T>, Polygon<T>> split(const Point<T>& a, const Point<T>& b) const {
        Polygon<T> left, right;
        for (int i = 0; i < size(); i++) {
            Point<T> cur = pts[i];
            Point<T> nxt = pts[(i + 1) % size()];
            double cross1 = (b - a).cross(cur - a);
            double cross2 = (b - a).cross(nxt - a);
            
            if (cross1 >= -eps) left.add_point(cur);
            if (cross1 <= eps) right.add_point(cur);
            
            if (cross1 * cross2 < -eps) {
                double t = (b - a).cross(a - cur) / (b - a).cross(nxt - cur);
                Point<T> intersect = cur + (nxt - cur) * t;
                left.add_point(intersect);
                right.add_point(intersect);
            }
        }
        return {left, right};
    }

};

using poly = Polygon<double>;
