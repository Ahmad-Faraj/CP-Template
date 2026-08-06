# 8. Half-Plane Intersection

## 8.1 $O(N \log N)$ Half-Plane Intersection
**Description:** Given $N$ directed lines, each defining a half-plane (the left side of the line), this algorithm computes their intersection. The intersection of half-planes is always a **convex polygon** (or empty/unbounded).
- **Core Idea:** Sort the half-planes by their polar angle. Maintain a double-ended queue (`deque`) of half-planes that form the convex boundary. 
- **Application:** Finding the core of a polygon, checking if a circle of radius $R$ can fit inside a polygon (shift edges inward by $R$, then run HPI).

<svg viewBox="0 0 400 250" xmlns="http://www.w3.org/2000/svg" style="background-color: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; font-family: sans-serif;">
  <pattern id="gridhp" width="20" height="20" patternUnits="userSpaceOnUse">
    <path d="M 20 0 L 0 0 0 20" fill="none" stroke="#e9ecef" stroke-width="1"/>
  </pattern>
  <rect width="400" height="250" fill="url(#gridhp)" />
  
  <!-- Intersection Core -->
  <polygon points="150,50 250,80 200,180 120,130" fill="rgba(46, 204, 113, 0.3)" stroke="#27ae60" stroke-width="3" stroke-linejoin="round"/>
  
  <!-- Edge 1: (150,50) to (250,80). y = 0.3x + 5. Line: x=50->y=20, x=350->y=110 -->
  <line x1="50" y1="20" x2="350" y2="110" stroke="#2c3e50" stroke-width="2"/>
  <polyline points="200,65 203,79 214,68" fill="#2c3e50"/>
  
  <!-- Edge 2: (250,80) to (200,180). y = -2x + 580. Line: x=280->y=20, x=170->y=240 -->
  <line x1="280" y1="20" x2="170" y2="240" stroke="#2c3e50" stroke-width="2"/>
  <polyline points="225,130 211,133 218,121" fill="#2c3e50"/>
  
  <!-- Edge 3: (200,180) to (120,130). y = 0.625x + 55. Line: x=280->y=230, x=40->y=80 -->
  <line x1="280" y1="230" x2="40" y2="80" stroke="#2c3e50" stroke-width="2"/>
  <polyline points="160,155 157,141 146,152" fill="#2c3e50"/>
  
  <!-- Edge 4: (120,130) to (150,50). y = -2.66x + 450. Line: x=100->y=183, x=170->y=-3.3 -->
  <line x1="100" y1="183.3" x2="170" y2="-3.3" stroke="#2c3e50" stroke-width="2"/>
  <polyline points="135,90 149,87 142,99" fill="#2c3e50"/>
  
  <text x="185" y="125" fill="#27ae60" font-weight="bold" font-size="13" text-anchor="middle">Intersection Core</text>
</svg>

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"
#include "HalfPlane.cpp"
#include "../Polygons/Polygon.cpp"

// Returns the exact intersection point of the boundaries of two half-planes.
pt hpIntersection(const hp& a, const hp& b) {
    double cross = a.dir.cross(b.dir);
    if (abs(cross) < eps) return pt(1e18, 1e18); // Parallel
    double t = (b.p - a.p).cross(b.dir) / cross;
    return a.p + (a.dir * t);
}

// O ( N log N )
// Returns the convex polygon formed by the intersection of the half-planes.
poly halfPlaneIntersection(vector<hp>& H) {
    // 1. Add bounding box half-planes to ensure the intersection is bounded
    double BBOX = 1e9;
    H.push_back(hp(pt(-BBOX, -BBOX), pt(BBOX, -BBOX)));
    H.push_back(hp(pt(BBOX, -BBOX), pt(BBOX, BBOX)));
    H.push_back(hp(pt(BBOX, BBOX), pt(-BBOX, BBOX)));
    H.push_back(hp(pt(-BBOX, BBOX), pt(-BBOX, -BBOX)));

    // 2. Sort half-planes by angle
    sort(H.begin(), H.end());

    // 3. Remove parallel half-planes (keep the one that is strictly "innermost")
    vector<hp> C;
    for (int i = 0; i < H.size(); i++) {
        if (!C.empty() && abs(H[i].angle - C.back().angle) < eps) {
            if (H[i].out(C.back().p)) C.back() = H[i]; 
        } else {
            C.push_back(H[i]);
        }
    }

    // 4. Deque logic
    deque<hp> dq;
    deque<pt> p; // intersection points of consecutive half-planes in deque

    for (int i = 0; i < C.size(); i++) {
        // Pop from back if the intersection of the last two is outside the current half-plane
        while (dq.size() >= 2 && C[i].out(p.back())) {
            dq.pop_back();
            p.pop_back();
        }
        // Pop from front if the intersection of the first two is outside the current half-plane
        while (dq.size() >= 2 && C[i].out(p.front())) {
            dq.pop_front();
            p.pop_front();
        }
        
        // Add current half plane
        dq.push_back(C[i]);
        if (dq.size() >= 2) {
            p.push_back(hpIntersection(dq[dq.size() - 2], dq.back()));
        }
    }

    // 5. Final cleanup for the closing edge
    while (dq.size() >= 3 && dq.front().out(p.back())) {
        dq.pop_back();
        p.pop_back();
    }
    while (dq.size() >= 3 && dq.back().out(p.front())) {
        dq.pop_front();
        p.pop_front();
    }

    // 6. Form the final polygon
    if (dq.size() < 3) return poly(); // Empty intersection
    
    poly res;
    p.push_back(hpIntersection(dq.back(), dq.front()));
    for (auto point : p) res.add_point(point);
    
    return res;
}
```
