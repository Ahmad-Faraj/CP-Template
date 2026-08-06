# 5. Triangles

## 5.0 Mathematical Properties & Rules
Triangles are the most fundamental 2D shapes. Almost all complex polygon geometry can be reduced to triangulations.

**The Great Laws:**
- **Law of Sines:** $\frac{a}{\sin(A)} = \frac{b}{\sin(B)} = \frac{c}{\sin(C)} = 2R$ (where $R$ is the radius of the Circumcircle).
- **Law of Cosines:** $c^2 = a^2 + b^2 - 2ab \cos(C)$. (Generalization of the Pythagorean theorem).
- **Triangle Inequality:** The sum of any two sides must strictly exceed the third side: $a + b > c$.

**Area Formulas:**
- **Base & Height:** $\text{Area} = \frac{1}{2} \times b \times h$.
- **Two Sides & Angle:** $\text{Area} = \frac{1}{2} ab \sin(C)$.
- **Inradius Formula:** $\text{Area} = r \times s$, where $r$ is the Incircle radius, and $s = \frac{a+b+c}{2}$ is the semi-perimeter.
- **Heron's Formula:** $\text{Area} = \sqrt{s(s-a)(s-b)(s-c)}$.

**Theorems & Centers:**
- **Apollonius's Theorem:** Relates the length of a median $m$ to the sides: $a^2 + b^2 = 2(m^2 + (\frac{c}{2})^2)$.
- **Centroid:** Center of mass. Intersection of medians. Divides each median in a 2:1 ratio.
- **Circumcenter:** Center of circumcircle. Intersection of perpendicular bisectors.
- **Incenter:** Center of inscribed circle. Intersection of angle bisectors.
- **Orthocenter:** Intersection of altitudes.

---

Independent functions strictly dealing with Triangles. Triangles are represented either implicitly by 3 points, or mathematically via side lengths.

## 5.1 Triangle Centers (Circumcenter & Centroid)
**Description:** 
- **Circumcenter:** The center of the unique circle passing through all 3 vertices. Found by the intersection of perpendicular bisectors.
- **Centroid:** The center of mass. A simple average of the 3 coordinates!

<svg viewBox="0 0 400 250" xmlns="http://www.w3.org/2000/svg" style="background-color: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; font-family: sans-serif;">
  <pattern id="grid8" width="20" height="20" patternUnits="userSpaceOnUse">
    <path d="M 20 0 L 0 0 0 20" fill="none" stroke="#e9ecef" stroke-width="1"/>
  </pattern>
  <rect width="400" height="250" fill="url(#grid8)" />
  
  <!-- Triangle -->
  <polygon points="120,50 300,100 80,200" fill="rgba(52, 152, 219, 0.2)" stroke="#2c3e50" stroke-width="3" stroke-linejoin="round"/>
  
  <!-- Circumcircle -->
  <circle cx="187" cy="120" r="95" fill="none" stroke="#e74c3c" stroke-width="2" stroke-dasharray="6,4"/>
  <circle cx="187" cy="120" r="4" fill="#e74c3c"/>
  <text x="200" y="125" fill="#e74c3c" font-weight="bold">Circumcenter</text>
  
  <!-- Centroid -->
  <circle cx="166" cy="116" r="4" fill="#27ae60"/>
  <text x="95" y="115" fill="#27ae60" font-weight="bold">Centroid</text>
  
  <!-- Vertices -->
  <circle cx="120" cy="50" r="5" fill="#2c3e50"/>
  <circle cx="300" cy="100" r="5" fill="#2c3e50"/>
  <circle cx="80" cy="200" r="5" fill="#2c3e50"/>
</svg>

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"

// Returns the Circumcenter and the Circumradius of Triangle ABC.
pair<double, pt> circumCircle(const pt& a, const pt& b, const pt& c) {
    double a1 = 2 * (b.x - a.x);
    double b1 = 2 * (b.y - a.y);
    double c1 = b.norm() - a.norm();
    
    double a2 = 2 * (c.x - b.x);
    double b2 = 2 * (c.y - b.y);
    double c2 = c.norm() - b.norm();
    
    double det = a1 * b2 - a2 * b1;
    if (abs(det) < eps) return {1e18, pt(1e18, 1e18)}; // Collinear points
    
    double cx = (c1 * b2 - c2 * b1) / det;
    double cy = (a1 * c2 - a2 * c1) / det;
    
    pt center(cx, cy);
    return {center.distance(a), center}; // {Radius, Center}
}

// Returns the centroid (center of mass) of the triangle.
pt triangleCentroid(const pt& a, const pt& b, const pt& c) {
    return (a + b + c) / 3.0;
}
```

## 5.2 Triangle Area (Heron's Formula)
**Description:** Finding the area of a triangle given only its 3 side lengths $a, b, c$.
- $s = \frac{a+b+c}{2}$ (Semi-perimeter)
- $Area = \sqrt{s(s-a)(s-b)(s-c)}$

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"

// Returns the area using Heron's formula to avoid precision loss of cross products on large coordinates.
double triangleArea(double a, double b, double c) {
    double s = (a + b + c) / 2.0;
    return sqrt(max(0.0, s * (s - a) * (s - b) * (s - c)));
}
```
