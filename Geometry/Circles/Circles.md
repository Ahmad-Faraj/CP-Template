# 3. Circles

## 3.0 Mathematical Properties & Rules
Crucial circle theorems required to solve angles, lengths, and intersection problems without brute-force geometry:

**Basic Measurements:**
- **Circumference & Area:** $C = 2\pi R$, $A = \pi R^2$.
- **Arc Length:** For a central angle $\theta$ (in radians), $L = R \times \theta$.
- **Sector Area:** The area of a pie slice with central angle $\theta$ is $A = \frac{1}{2} R^2 \theta$.
- **Chord Length:** A chord formed by a central angle $\theta$ has length $2R \sin(\frac{\theta}{2})$.

**Theorems of Intersecting Lines:**
- **Intersecting Chords Theorem:** If two chords intersect inside a circle at point $P$, forming segments $(A, B)$ and $(C, D)$, then $P_A \times P_B = P_C \times P_D$.
- **Tangent-Secant Theorem:** If a tangent segment from exterior point $P$ touches the circle at $T$, and a secant from $P$ cuts the circle at $A$ and $B$, then $PT^2 = PA \times PB$.

**Angle Theorems:**
- **Inscribed Angle Theorem:** An angle $\theta$ inscribed in a circle is exactly half of the central angle $2\theta$ that subtends the same arc. 
- **Thales's Theorem:** Any angle inscribed inside a semicircle is exactly a right angle ($90^\circ$).

---
Independent functions for geometric operations on circles. Represents a Circle as a center point `cen` and radius `rad`.

## 3.1 Circle-Line Intersection
**Description:** Finds the exact points where an infinite line intersects a circle. The line is defined by two points $P_0, P_1$.

<svg viewBox="0 0 400 250" xmlns="http://www.w3.org/2000/svg" style="background-color: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; font-family: sans-serif;">
  <pattern id="grid6" width="20" height="20" patternUnits="userSpaceOnUse">
    <path d="M 20 0 L 0 0 0 20" fill="none" stroke="#e9ecef" stroke-width="1"/>
  </pattern>
  <rect width="400" height="250" fill="url(#grid6)" />
  
  <!-- Circle -->
  <circle cx="200" cy="125" r="80" fill="rgba(52, 152, 219, 0.1)" stroke="#3498db" stroke-width="3"/>
  <circle cx="200" cy="125" r="4" fill="#2c3e50"/>
  
  <!-- Secant -->
  <line x1="50" y1="80" x2="350" y2="80" stroke="#e74c3c" stroke-width="2"/>
  <text x="50" y="72" fill="#e74c3c" font-weight="bold">2 Intersections (Secant)</text>
  <circle cx="133.86" cy="80" r="5" fill="#e74c3c"/>
  <circle cx="266.14" cy="80" r="5" fill="#e74c3c"/>
  
  <!-- Tangent -->
  <line x1="50" y1="205" x2="350" y2="205" stroke="#27ae60" stroke-width="2"/>
  <text x="50" y="222" fill="#27ae60" font-weight="bold">1 Intersection (Tangent)</text>
  <circle cx="200" cy="205" r="5" fill="#27ae60"/>
  
  <!-- Disjoint -->
  <line x1="320" y1="30" x2="380" y2="230" stroke="#34495e" stroke-width="2"/>
  <text x="335" y="45" fill="#34495e" font-weight="bold">0 (Disjoint)</text>
</svg>

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"

// Returns the number of intersection points (0, 1, or 2).
// Points are stored in r1 and r2.
int circleLineIntersection(const pt& p0, const pt& p1, const pt& cen, double rad, pt& r1, pt& r2) {
    if (p0 == p1) return 0; // Invalid line
    
    double a = (p1 - p0).dot(p1 - p0);
    double b = 2 * (p1 - p0).dot(p0 - cen);
    double c = (p0 - cen).dot(p0 - cen) - rad * rad;
    
    double det = b * b - 4 * a * c;
    if (det < -eps) return 0; // 0 intersections
    
    int res = 2;
    if (abs(det) < eps) {
        det = 0;
        res = 1; // 1 intersection (Tangent)
    }
    
    det = sqrt(det);
    double t1 = (-b + det) / (2 * a);
    double t2 = (-b - det) / (2 * a);
    
    r1 = p0 + (p1 - p0) * t1;
    // Both return points with respect to the original segment.
    return {p1, p2}; 
}
```

## 3.3 Circle-Circle Intersection Area
**Description:** Calculates the exact overlapping area of two circles.

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"
#include "Circle.cpp"

// Returns the intersecting area of two circles.
double circleIntersectionArea(const Circle<double>& c1, const Circle<double>& c2) {
    double d = c1.c.distance(c2.c);
    
    // 1. No overlap
    if (d >= c1.r + c2.r) return 0.0;
    
    // 2. One strictly inside the other
    if (d <= abs(c1.r - c2.r)) {
        double min_r = min(c1.r, c2.r);
        return acos(-1.0) * min_r * min_r;
    }
    
    // 3. Partial overlap (Using Law of Cosines to find sector angles)
    double angle1 = 2 * acos((c1.r * c1.r + d * d - c2.r * c2.r) / (2 * c1.r * d));
    double angle2 = 2 * acos((c2.r * c2.r + d * d - c1.r * c1.r) / (2 * c2.r * d));
    
    double area1 = 0.5 * c1.r * c1.r * (angle1 - sin(angle1));
    double area2 = 0.5 * c2.r * c2.r * (angle2 - sin(angle2));
    
    return area1 + area2;
}
```

## 3.4 Common Tangents of Two Circles
**Description:** Finding the lines that are tangent to two circles simultaneously (Internal and External tangents).

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"
#include "Circle.cpp"
#include "../Points_and_Lines/Line.cpp"

// Adds tangents to the vector 'ans'
// inner == 1 for internal tangents, -1 for external tangents
void getTangents(const Circle<double>& c1, const Circle<double>& c2, int inner, vector<Line<double>>& ans) {
    double d = c1.c.distance(c2.c);
    double dr = c1.r - c2.r * inner;
    
    if (d < abs(dr)) return; // One circle is completely inside the other
    
    double base_angle = atan2(c2.c.y - c1.c.y, c2.c.x - c1.c.x);
    double offset = asin(dr / d);
    
    for (int sign : {-1, 1}) {
        double angle = base_angle + sign * offset;
        pt dir(cos(angle), sin(angle));
        
        pt p1 = c1.c + dir * (c1.r * sign * (inner == 1 ? -1 : 1) );
        // The tangent is perpendicular to the radius
        pt p2 = p1 + pt(-dir.y, dir.x); 
        
        ans.push_back(Line<double>(p1, p2));
        
        // If circles touch at exactly one point, there is only one tangent of this type
        if (abs(d - abs(dr)) < eps) break;
    }
}
```

## 3.5 Minimum Enclosing Circle (Welzl's Algorithm)
**Description:** Finds the smallest mathematical circle that strictly encloses a set of $N$ points in expected $O(N)$ time using randomization.

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"
#include "Circle.cpp"
#include <algorithm>
#include <random>

// Helper to check if a point is inside the circle
bool inCircle(const Circle<double>& c, const Point<double>& p) {
    return c.c.distance(p) <= c.r + eps;
}

// Computes Minimum Enclosing Circle for a strict boundary of up to 3 points
Circle<double> MEC_boundary(vector<Point<double>>& R) {
    if (R.empty()) return Circle<double>(pt(0, 0), 0);
    if (R.size() == 1) return Circle<double>(R[0], 0);
    if (R.size() == 2) return Circle<double>::from2Points(R[0], R[1]);
    return Circle<double>::from3Points(R[0], R[1], R[2]);
}

// Welzl's recursive solver
Circle<double> welzl(vector<Point<double>>& P, vector<Point<double>> R, int n) {
    if (n == 0 || R.size() == 3) {
        return MEC_boundary(R);
    }
    
    // Get a random point
    Point<double> p = P[n - 1];
    Circle<double> c = welzl(P, R, n - 1);
    
    // If the point is inside, the MEC is still valid
    if (inCircle(c, p)) return c;
    
    // Otherwise, p MUST be on the boundary of the MEC
    R.push_back(p);
    return welzl(P, R, n - 1);
}

// Main function: O(N) expected time
Circle<double> minEnclosingCircle(vector<Point<double>> P) {
    mt19937 rng(1337);
    shuffle(P.begin(), P.end(), rng);
    return welzl(P, vector<Point<double>>(), P.size());
}
```

