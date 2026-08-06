# 2. Polygons

## 2.0 Mathematical Properties & Rules
Fundamental theorems and formulas regarding polygons that frequently appear in mathematical CP problems:

**Angles:**
- **Sum of Internal Angles:** For a simple polygon with $N$ vertices, the sum is $(N - 2) \times 180^\circ$ (or $(N - 2)\pi$ radians).
- **Sum of External Angles:** For *any* simple convex polygon, the sum of external angles is exactly $360^\circ$ ($2\pi$).
- **Regular Polygon Internal Angle:** $\frac{(N - 2) \times 180^\circ}{N}$.

**Diagonals & Areas:**
- **Number of Diagonals:** $\frac{N(N - 3)}{2}$.
- **Regular Polygon Area:** If a regular polygon has side length $S$, its area is $\frac{1}{4} N S^2 \cot(\frac{\pi}{N})$.
- **Shoelace Theorem:** The area of any simple polygon is $\frac{1}{2} | \sum (x_i y_{i+1} - x_{i+1} y_i) |$.

**Euler's Formula for Planar Graphs:**
- If polygon edges are treated as a graph that splits the plane into regions: $V - E + F = 1 + C$.
  - $V = \text{Vertices}$, $E = \text{Edges}$, $F = \text{Faces (regions)}$, $C = \text{Connected Components}$.
  - (Often $V - E + F = 2$ if we count the infinite outside face and the graph is fully connected).

---
This file contains independent functions operating on polygons. All functions rely on the base `Point` and `Polygon` structures. A polygon is represented as `poly`.

## 2.1 Polygon Area
**Description:** Calculates the exact area of any simple (non-self-intersecting) polygon using the Shoelace Formula (Cross Product sum).

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"
#include "Polygon.cpp"

// Returns the absolute area of a polygon. 
// O(N) where N is the number of vertices.
double polygonArea(const poly& v) {
    double area = 0;
    int n = v.size();
    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;
        area += v[i].cross(v[next]);
    }
    return abs(area / 2.0);
}
```

## 2.2 Point Inside Polygon (Winding Number)
**Description:** Determines if a point $P$ is strictly inside a given polygon using the Ray Casting / Winding Number algorithm.

<svg viewBox="0 0 400 250" xmlns="http://www.w3.org/2000/svg" style="background-color: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; font-family: sans-serif;">
  <pattern id="grid3" width="20" height="20" patternUnits="userSpaceOnUse">
    <path d="M 20 0 L 0 0 0 20" fill="none" stroke="#e9ecef" stroke-width="1"/>
  </pattern>
  <rect width="400" height="250" fill="url(#grid3)" />
  
  <!-- Polygon -->
  <polygon points="120,60 260,100 220,200 80,160" fill="rgba(52, 152, 219, 0.3)" stroke="#3498db" stroke-width="3" stroke-linejoin="round"/>
  
  <!-- Point P inside -->
  <circle cx="160" cy="120" r="5" fill="#e74c3c"/>
  <text x="150" y="110" fill="#e74c3c" font-weight="bold" text-anchor="end">P</text>
  
  <!-- Ray -->
  <line x1="160" y1="120" x2="380" y2="120" stroke="#e74c3c" stroke-dasharray="8,4" stroke-width="2"/>
  <polygon points="380,120 370,115 370,125" fill="#e74c3c"/>
  
  <!-- Intersection Highlight (Exactly on edge mathematically) -->
  <circle cx="252" cy="120" r="5" fill="#2c3e50"/>
  <text x="260" y="110" fill="#2c3e50" font-weight="bold" font-size="14">1 Cut (Odd = Inside)</text>
</svg>

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"
#include "Polygon.cpp"

// Returns true if the point P is inside the polygon V.
// O(N) where N is the number of vertices.
bool isInsidePolygon(const poly& v, const pt& p) {
    int wn = 0; // Winding number
    int cur = v.size() - 1;
    for (int nxt = 0; nxt < v.size(); nxt++) {
        // If the point lies exactly on the boundary segment
        if (pointSegmentDist(v[cur], v[nxt], p) < eps) return true; // Or false depending on problem rules!
        
        // Ray casting to the right
        if (v[cur].y <= p.y) { // upward crossing
            if (v[nxt].y > p.y && (v[nxt] - v[cur]).cross(p - v[cur]) > eps)
                wn++;
        } else { // downward crossing
            if (v[nxt].y <= p.y && (v[nxt] - v[cur]).cross(p - v[cur]) < -eps)
                wn--;
        }
        cur = nxt;
    }
    return wn != 0;
}
```

## 2.3 Cut Polygon (Convex Cut)
**Description:** Slices a convex polygon with an infinite line defined by two points $A$ and $B$, returning the new polygon that lies on the **left side** of the line $AB$.

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"
#include "Polygon.cpp"

// Slices polygon Q with line AB. Returns the part of Q on the left of vector AB.
poly cutPolygon(const pt& a, const pt& b, const poly& Q) {
    poly p;
    for (int i = 0; i < Q.size(); ++i) {
        double left1 = (b - a).cross(Q[i] - a);
        double left2 = 0;
        if (i != Q.size() - 1) left2 = (b - a).cross(Q[i + 1] - a);
        else left2 = (b - a).cross(Q[0] - a); // wrap around
        
        // If point is on the left side, keep it
        if (left1 > -eps) p.add_point(Q[i]);
        
        // If the segment crosses the cutting line, push the intersection point
        if (left1 * left2 < -eps) {
            pt next_pt = (i == Q.size() - 1) ? Q[0] : Q[i + 1];
            p.add_point(lineLineIntersection(Q[i], next_pt, a, b));
        }
    }
    return p;
}
```

## 2.4 Pick's Theorem
**Description:** For polygons where all vertices lie exactly on integer lattice points.
Formula: $Area = I + \frac{B}{2} - 1$, where $I$ is interior points and $B$ is boundary points.

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"
#include "Polygon.cpp"

// Returns the number of strictly interior integer points of a lattice polygon.
long long picksTheoremInterior(const Polygon<long long>& p) {
    long long area2 = 0; // 2 * Area
    long long boundary = 0;
    int n = p.size();
    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;
        area2 += p[i].cross(p[next]);
        
        pti diff = p[next] - p[i];
        boundary += abs(__gcd(diff.x, diff.y)); // Number of points on segment
    }
    area2 = abs(area2);
    // Area = I + B/2 - 1 => 2*Area = 2I + B - 2 => 2I = 2*Area - B + 2
    return (area2 - boundary + 2) / 2;
}
```

## 2.5 Point in Convex Polygon in $O(\log N)$
**Description:** If a polygon is strictly convex, we can determine if a query point $Q$ is inside it in logarithmic time! 
- We pick an anchor vertex $P[0]$.
- We binary search the angle $Q$ makes with $P[0]$ relative to the polygon edges.
- Once we find the angular wedge $P[0] \to P[i] \to P[i+1]$ that $Q$ belongs to, we just do a CCW check on that triangle!

<svg viewBox="0 0 400 250" xmlns="http://www.w3.org/2000/svg" style="background-color: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; font-family: sans-serif;">
  <pattern id="grid4" width="20" height="20" patternUnits="userSpaceOnUse">
    <path d="M 20 0 L 0 0 0 20" fill="none" stroke="#e9ecef" stroke-width="1"/>
  </pattern>
  <rect width="400" height="250" fill="url(#grid4)" />
  
  <!-- Polygon -->
  <polygon points="60,200 320,200 260,60 120,40" fill="rgba(52, 152, 219, 0.1)" stroke="#3498db" stroke-width="2"/>
  
  <!-- Anchor P0 -->
  <circle cx="60" cy="200" r="5" fill="#2c3e50"/>
  <text x="50" y="215" font-weight="bold" fill="#2c3e50" text-anchor="end">P[0]</text>
  
  <!-- Radial lines dividing wedges -->
  <line x1="60" y1="200" x2="320" y2="200" stroke="#95a5a6" stroke-dasharray="4,4"/>
  <line x1="60" y1="200" x2="260" y2="60" stroke="#95a5a6" stroke-dasharray="4,4"/>
  <line x1="60" y1="200" x2="120" y2="40" stroke="#95a5a6" stroke-dasharray="4,4"/>
  
  <!-- Query Point Q -->
  <circle cx="200" cy="130" r="6" fill="#e74c3c"/>
  <text x="210" y="125" fill="#e74c3c" font-weight="bold">Query Q</text>
  <line x1="60" y1="200" x2="200" y2="130" stroke="#e74c3c" stroke-width="2"/>
  
  <!-- Precise Angle Arc -->
  <path d="M 160 200 A 100 100 0 0 0 149.4 155.3" fill="none" stroke="#e74c3c" stroke-width="3"/>
  <text x="175" y="175" fill="#e74c3c" font-weight="bold" font-size="14">Binary Search Angle</text>
</svg>

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"
#include "Polygon.cpp"

// Returns true if Point q is strictly inside the CONVEX polygon poly.
// Precondition: poly must be convex and vertices sorted in CCW order!
// O ( log N )
bool pointInConvexLogN(const poly& p, const pt& q) {
    int n = p.size();
    if (n < 3) return false;
    
    // Check if it's completely outside the angular range of the polygon from p[0]
    if (ccw(p[0], p[1], q) < 0 || ccw(p[0], p[n - 1], q) > 0) return false;
    
    // Binary search for the wedge
    int low = 1, high = n - 1, idx = 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (ccw(p[0], p[mid], q) >= 0) {
            idx = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    
    // q is inside the wedge P[0] - P[idx] - P[idx+1]
    if (idx == n - 1) {
        // If it lies exactly on the last ray, check segment distance
        return pointSegmentDist(p[0], p[n-1], q) < eps;
    }
    
    // To be inside the polygon, it must be on the left of the segment P[idx] -> P[idx+1]
    return ccw(p[idx], p[idx + 1], q) >= 0;
}
```

## 2.6 Rotating Calipers (Polygon Diameter)
**Description:** Finds the maximum distance between any two points in a Convex Polygon in $O(N)$ time. 
- Visually, you place the polygon between two parallel lines (calipers).
- You rotate the calipers around the polygon. The furthest points are the ones touching the calipers!

<svg viewBox="0 0 400 250" xmlns="http://www.w3.org/2000/svg" style="background-color: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; font-family: sans-serif;">
  <pattern id="grid5" width="20" height="20" patternUnits="userSpaceOnUse">
    <path d="M 20 0 L 0 0 0 20" fill="none" stroke="#e9ecef" stroke-width="1"/>
  </pattern>
  <rect width="400" height="250" fill="url(#grid5)" />
  
  <polygon points="120,60 260,100 300,180 180,220 60,160" fill="rgba(46, 204, 113, 0.2)" stroke="#27ae60" stroke-width="3" stroke-linejoin="round"/>
  
  <!-- Parallel Tangents (Calipers) Math: y = -1.5x + C -->
  <line x1="40" y1="180" x2="160" y2="0" stroke="#34495e" stroke-width="2" stroke-dasharray="6,4"/>
  <line x1="260" y1="240" x2="380" y2="60" stroke="#34495e" stroke-width="2" stroke-dasharray="6,4"/>
  
  <!-- Diameter line -->
  <line x1="120" y1="60" x2="300" y2="180" stroke="#e74c3c" stroke-width="3"/>
  <circle cx="120" cy="60" r="5" fill="#e74c3c"/>
  <circle cx="300" cy="180" r="5" fill="#e74c3c"/>
  <text x="210" y="110" fill="#e74c3c" font-weight="bold" font-size="16" transform="rotate(33.6 210,110)" text-anchor="middle">Max Diameter</text>
</svg>

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"
#include "Polygon.cpp"

// Returns the maximum distance squared between any two points in a convex polygon.
// O ( N )
double rotatingCalipersDiameter(const poly& p) {
    int n = p.size();
    if (n < 2) return 0;
    if (n == 2) return p[0].distance(p[1]);
    
    int k = 1;
    // Find the furthest vertex from the edge p[n-1] -> p[0]
    while (abs((p[0] - p[n - 1]).cross(p[k + 1] - p[n - 1])) > 
           abs((p[0] - p[n - 1]).cross(p[k] - p[n - 1]))) {
        k = (k + 1) % n;
    }
    
    double max_dist = 0;
    for (int i = 0, j = k; i <= k && j < n; i++) {
        max_dist = max(max_dist, p[i].distance(p[j]));
        // Rotate calipers
        while (j < n && abs((p[i + 1] - p[i]).cross(p[(j + 1) % n] - p[i])) > 
                        abs((p[i + 1] - p[i]).cross(p[j] - p[i]))) {
            max_dist = max(max_dist, p[i].distance(p[(j + 1) % n]));
            j++;
        }
    }
    // Both tangent indices found, compute diameter distance!
    return sqrt(max_dist);
}
```

## 2.6 Polygon Cut
**Description:** Cuts a convex polygon by a directed line $AB$ and returns the exact sub-polygon strictly on the left side of the line in $O(N)$. Useful for Voronoi cells.

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"
#include "Polygon.cpp"

poly cutPolygon(const poly& P, pt a, pt b) {
    poly res;
    for (int i = 0; i < P.size(); i++) {
        double cross1 = (b - a).cross(P[i] - a);
        double cross2 = (b - a).cross(P[(i + 1) % P.size()] - a);
        
        // If current point is on the left side, it belongs in the cut
        if (cross1 > -eps) res.add_point(P[i]);
        
        // If the edge crosses the cutting line, add the exact intersection point
        if (cross1 * cross2 < -eps) {
            pt p1 = P[i], p2 = P[(i + 1) % P.size()];
            double t = (b - a).cross(a - p1) / (b - a).cross(p2 - p1);
            res.add_point(p1 + (p2 - p1) * t);
        }
    }
    return res;
}
```

## 2.7 Minkowski Sum
**Description:** Computes the set of all points $A + B$ where $A \in P$ and $B \in Q$. For convex polygons of sizes $N$ and $M$, this computes the exact resulting convex polygon in strict $O(N + M)$ time. Used in collision detection and bounding queries.

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"
#include "Polygon.cpp"
#include <algorithm>

// Reorders a convex polygon so its bottom-leftmost vertex is first
void reorderPolygon(poly& P) {
    int pos = 0;
    for (int i = 1; i < P.size(); i++) {
        if (P[i].y < P[pos].y || (abs(P[i].y - P[pos].y) < eps && P[i].x < P[pos].x)) pos = i;
    }
    rotate(P.pts.begin(), P.pts.begin() + pos, P.pts.end());
}

poly minkowskiSum(poly P, poly Q) {
    reorderPolygon(P);
    reorderPolygon(Q);
    
    // Add first points to the end to simulate cyclic edge logic
    P.add_point(P[0]);
    Q.add_point(Q[0]);
    
    poly res;
    int i = 0, j = 0;
    while (i < P.size() - 1 || j < Q.size() - 1) {
        res.add_point(P[i] + Q[j]);
        
        pt e1 = P[i + 1] - P[i];
        pt e2 = Q[j + 1] - Q[j];
        
        double cross = e1.cross(e2);
        
        // Advance the pointer of the edge that "turns" earlier
        if (cross > eps) i++;
        else if (cross < -eps) j++;
        else { i++; j++; } // Edges are collinear
    }
    return res;
}
```
