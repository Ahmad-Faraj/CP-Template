# 1. Points, Vectors, and Lines

## 1.0 Mathematical Properties & Rules
Before jumping into the code, here are the fundamental geometric properties and formulas associated with Points, Lines, and Vectors:

**Line Equations:**
- **Standard Form:** $Ax + By + C = 0$. (Normal vector to the line is $\langle A, B \rangle$).
- **Slope-Intercept Form:** $y = mx + c$.
- **Two-Point Form:** $(y - y_1) = m(x - x_1)$ where $m = \frac{y_2 - y_1}{x_2 - x_1}$.

**Vector Operations:**
- **Dot Product ($A \cdot B$):** $|A||B|\cos(\theta)$. 
  - If $A \cdot B = 0$, the vectors are strictly **orthogonal** (perpendicular).
  - If $A \cdot B > 0$, the angle between them is acute ($< 90^\circ$).
  - If $A \cdot B < 0$, the angle between them is obtuse ($> 90^\circ$).
- **Cross Product ($A \times B$):** $|A||B|\sin(\theta)$.
  - If $A \times B = 0$, the vectors are strictly **collinear** (parallel).
  - Represents the **directed area** of the parallelogram formed by $A$ and $B$.
  - Used to determine orientation: $>0$ means $B$ is counter-clockwise from $A$ (Left turn).

**Distance & Angles:**
- **Point to Line Formula:** For point $(x_0, y_0)$ and line $Ax+By+C=0$, $d = \frac{|Ax_0 + By_0 + C|}{\sqrt{A^2 + B^2}}$.
- **Angle Between Two Lines:** For lines with slopes $m_1, m_2$, the angle $\theta$ between them is given by $\tan(\theta) = \left| \frac{m_1 - m_2}{1 + m_1 m_2} \right|$.

---

This file contains independent, modular functions for interacting with Points and Lines. All functions depend strictly on the base `Point` structure defined in `Primitives/Point.cpp`.

## 1.1 CCW (Counter-Clockwise) Test
**Description:** Determines the orientation of three points $A$, $B$, and $C$. It tells us if walking from $A \to B \to C$ is a left turn, right turn, or straight line.

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"

// Returns:
//  1 if C is strictly Counter-Clockwise (Left) of AB
// -1 if C is strictly Clockwise (Right) of AB
//  0 if A, B, C are collinear
template<typename T>
int ccw(const Point<T>& a, const Point<T>& b, const Point<T>& c) {
    T cross_product = a.cross(b, c);
    if (cross_product > eps) return 1;
    if (cross_product < -eps) return -1;
    return 0;
}
```

## 1.2 Point to Line Distance
**Description:** Calculates the shortest orthogonal distance from a point $P$ to an infinite line defined by two points $A$ and $B$.

<svg viewBox="0 0 400 250" xmlns="http://www.w3.org/2000/svg" style="background-color: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; font-family: sans-serif;">
  <pattern id="grid1" width="20" height="20" patternUnits="userSpaceOnUse">
    <path d="M 20 0 L 0 0 0 20" fill="none" stroke="#e9ecef" stroke-width="1"/>
  </pattern>
  <rect width="400" height="250" fill="url(#grid1)" />
  
  <!-- Line AB -->
  <line x1="50" y1="200" x2="350" y2="50" stroke="#2c3e50" stroke-width="2"/>
  <circle cx="150" cy="150" r="5" fill="#2c3e50"/>
  <text x="150" y="165" fill="#2c3e50" font-weight="bold" text-anchor="middle">A</text>
  <circle cx="250" cy="100" r="5" fill="#2c3e50"/>
  <text x="250" y="85" fill="#2c3e50" font-weight="bold" text-anchor="middle">B</text>
  
  <!-- Point P -->
  <circle cx="200" cy="50" r="5" fill="#e74c3c"/>
  <text x="195" y="42" fill="#e74c3c" font-weight="bold" text-anchor="end">P</text>
  
  <!-- Projection -->
  <line x1="200" y1="50" x2="230" y2="110" stroke="#e74c3c" stroke-dasharray="6,4" stroke-width="2"/>
  <circle cx="230" cy="110" r="5" fill="#3498db"/>
  <text x="240" y="125" fill="#3498db" font-weight="bold">Proj(P)</text>
  
  <!-- Mathematical Right Angle -->
  <polyline points="221,114.5 216.6,105.6 225.5,101.1" fill="none" stroke="#e74c3c" stroke-width="1.5"/>
</svg>

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"

// Returns the shortest distance from point p to the infinite line passing through a and b.
double pointLineDist(const pt& a, const pt& b, const pt& p) {
    if (a == b) return a.distance(p);
    // The cross product geometrically represents the area of the parallelogram formed by AB and AP.
    // Area = base * height => height = Area / base
    return abs((b - a).cross(p - a)) / a.distance(b);
}
```

## 1.3 Point to Segment Distance
**Description:** Calculates the shortest distance from point $P$ to a **finite segment** $AB$. Unlike a line, if the orthogonal projection of $P$ falls outside the segment, the closest point is one of the endpoints $A$ or $B$.

<svg viewBox="0 0 400 250" xmlns="http://www.w3.org/2000/svg" style="background-color: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; font-family: sans-serif;">
  <pattern id="grid2" width="20" height="20" patternUnits="userSpaceOnUse">
    <path d="M 20 0 L 0 0 0 20" fill="none" stroke="#e9ecef" stroke-width="1"/>
  </pattern>
  <rect width="400" height="250" fill="url(#grid2)" />
  
  <!-- Segment AB -->
  <line x1="100" y1="150" x2="300" y2="150" stroke="#2c3e50" stroke-width="4" stroke-linecap="round"/>
  <circle cx="100" cy="150" r="5" fill="#2c3e50"/>
  <text x="100" y="170" fill="#2c3e50" font-weight="bold" text-anchor="middle">A</text>
  <circle cx="300" cy="150" r="5" fill="#2c3e50"/>
  <text x="300" y="170" fill="#2c3e50" font-weight="bold" text-anchor="middle">B</text>
  
  <!-- P1 inside -->
  <circle cx="200" cy="60" r="5" fill="#e74c3c"/>
  <text x="200" y="50" fill="#e74c3c" font-weight="bold" text-anchor="middle">P1</text>
  <line x1="200" y1="60" x2="200" y2="150" stroke="#e74c3c" stroke-dasharray="6,4" stroke-width="2"/>
  
  <!-- P2 outside -->
  <circle cx="360" cy="70" r="5" fill="#3498db"/>
  <text x="360" y="60" fill="#3498db" font-weight="bold" text-anchor="middle">P2</text>
  <line x1="360" y1="70" x2="300" y2="150" stroke="#3498db" stroke-dasharray="6,4" stroke-width="2"/>
</svg>

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"

// Returns the shortest distance from point p to the finite segment [a, b].
double pointSegmentDist(const pt& a, const pt& b, const pt& p) {
    if (a == b) return a.distance(p);
    
    // Check if the projection of p falls outside the segment past 'a'
    if ((b - a).dot(p - a) <= 0) return a.distance(p);
    
    // Check if the projection of p falls outside the segment past 'b'
    if ((a - b).dot(p - b) <= 0) return b.distance(p);
    
    // Projection is inside the segment, so it's identical to Point-to-Line distance
    return abs((b - a).cross(p - a)) / a.distance(b);
}
```

## 1.4 Line-Line Intersection
**Description:** Finds the exact intersection point of two infinite lines (Line 1 passing through $A, B$ and Line 2 passing through $C, D$). Uses Cramer's rule.

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"

// Returns the intersection point. If lines are parallel or collinear, returns a point with INF coordinates.
pt lineLineIntersection(const pt& a, const pt& b, const pt& c, const pt& d) {
    // Line 1: a1*x + b1*y = c1
    double a1 = b.y - a.y;
    double b1 = a.x - b.x;
    double c1 = a.x * b.y - b.x * a.y;
    
    // Line 2: a2*x + b2*y = c2
    double a2 = d.y - c.y;
    double b2 = c.x - d.x;
    double c2 = c.x * d.y - d.x * c.y;
    
    double det = a1 * b2 - a2 * b1;
    if (abs(det) < eps) {
        return pt(1e18, 1e18); // Parallel or Collinear
    }
    
    double x = (b2 * c1 - b1 * c2) / det;
    double y = (a1 * c2 - a2 * c1) / det;
    return pt(x, y);
}
```

## 1.5 Segment-Segment Intersection
**Description:** Determines if two finite segments $AB$ and $CD$ intersect. 
- The segments intersect if $A$ and $B$ are on opposite sides of line $CD$, AND $C$ and $D$ are on opposite sides of line $AB$.

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"

// Returns true if the finite segments [a, b] and [c, d] intersect.
// The intersection point is stored in 'out'.
bool segmentSegmentIntersection(const pt& a, const pt& b, const pt& c, const pt& d, pt& out) {
    double oa = ccw(c, d, a);
    double ob = ccw(c, d, b);
    double oc = ccw(a, b, c);
    double od = ccw(a, b, d);
    
    // If they straddle each other's lines, they strictly intersect
    if (oa * ob < 0 && oc * od < 0) {
        // Calculate exact point of intersection using interpolation
        out = (a * ob - b * oa) / (ob - oa);
        return true;
    }
    
    // (Optional edge case handling for collinear overlapping segments would go here)
    return false;
}
```
