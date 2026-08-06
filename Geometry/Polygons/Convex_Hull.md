# 4. Convex Hull

Independent functions for calculating the Convex Hull of a set of points. Relies on the base `Point` and `Polygon` structures.

## 4.1 Monotone Chain (Andrew's Algorithm)
**Description:** Computes the convex hull of a set of 2D points in $O(N \log N)$ time. It first sorts the points lexicographically (by X, then Y), and then constructs the upper and lower hulls separately using a stack. 

<svg viewBox="0 0 400 250" xmlns="http://www.w3.org/2000/svg" style="background-color: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; font-family: sans-serif;">
  <pattern id="grid7" width="20" height="20" patternUnits="userSpaceOnUse">
    <path d="M 20 0 L 0 0 0 20" fill="none" stroke="#e9ecef" stroke-width="1"/>
  </pattern>
  <rect width="400" height="250" fill="url(#grid7)" />
  
  <!-- Points -->
  <circle cx="80" cy="120" r="5" fill="#2c3e50"/>
  <circle cx="140" cy="180" r="5" fill="#2c3e50"/>
  <circle cx="260" cy="200" r="5" fill="#2c3e50"/>
  <circle cx="320" cy="100" r="5" fill="#2c3e50"/>
  <circle cx="200" cy="50" r="5" fill="#2c3e50"/>
  <circle cx="110" cy="70" r="5" fill="#2c3e50"/>
  <circle cx="200" cy="130" r="5" fill="#95a5a6"/>
  <circle cx="240" cy="150" r="5" fill="#95a5a6"/>
  <circle cx="160" cy="100" r="5" fill="#95a5a6"/>
  
  <!-- Lower Hull -->
  <polyline points="80,120 140,180 260,200 320,100" fill="none" stroke="#3498db" stroke-width="3" stroke-linejoin="round"/>
  <text x="160" y="220" fill="#3498db" font-weight="bold">Lower Hull</text>
  
  <!-- Upper Hull -->
  <polyline points="320,100 200,50 110,70 80,120" fill="none" stroke="#e74c3c" stroke-width="3" stroke-linejoin="round"/>
  <text x="160" y="35" fill="#e74c3c" font-weight="bold">Upper Hull</text>
</svg>

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"
#include "Polygon.cpp"

// Returns the convex hull of a set of points in counter-clockwise order as a Polygon struct.
// O ( N log N )
// If include_collinear is true, points that lie strictly on the edges of the hull will be included.
poly convexHull(vector<pt>& pnts, bool include_collinear = false) {
    if (pnts.size() <= 2) return poly(pnts);

    // Sort by X coordinate, then Y coordinate, and remove duplicates
    sort(pnts.begin(), pnts.end());
    pnts.erase(unique(pnts.begin(), pnts.end()), pnts.end());
    
    if (pnts.size() <= 2) return poly(pnts);
    
    vector<pt> hull;
    
    // Helper lambda for cross product turn direction
    auto turn = [](const pt& a, const pt& b, const pt& c) {
        return (b - a).cross(c - a);
    };
    
    // Lower Hull
    for (int i = 0; i < pnts.size(); ++i) {
        while (hull.size() >= 2) {
            double cross = turn(hull[hull.size() - 2], hull.back(), pnts[i]);
            // Exclude collinear: pop if cross <= 0 (right turn or straight)
            // Include collinear: pop if cross < 0 (strict right turn)
            if (include_collinear ? cross < -eps : cross <= eps) {
                hull.pop_back();
            } else {
                break;
            }
        }
        hull.push_back(pnts[i]);
    }
    
    // Upper Hull
    for (int i = pnts.size() - 2, t = hull.size() + 1; i >= 0; i--) {
        while (hull.size() >= t) {
            double cross = turn(hull[hull.size() - 2], hull.back(), pnts[i]);
            if (include_collinear ? cross < -eps : cross <= eps) {
                hull.pop_back();
            } else {
                break;
            }
        }
        hull.push_back(pnts[i]);
    }
    
    // The last point of upper hull is the first point of the lower hull
    hull.pop_back();
    return poly(hull);
}
```
