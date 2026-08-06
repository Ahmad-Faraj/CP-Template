# 7. Sweep Line Algorithms

Algorithms that rely on sorting events by X-coordinate and sweeping a vertical line across the plane to resolve geometrical queries efficiently.

## 7.1 Closest Pair of Points ($O(N \log N)$)
**Description:** Finds the absolute minimum distance between any two points in a 2D plane. 
- Using a Sweep Line, we keep an active `std::set` of points ordered by their Y-coordinates.
- As the line moves to the right, we only check the active points that are within a $d$-wide strip from the current point.
- Points that fall too far behind the sweep line (distance $> d$ in X) are erased from the set!

<svg viewBox="0 0 400 250" xmlns="http://www.w3.org/2000/svg" style="background-color: #f8f9fa; border: 1px solid #dee2e6; border-radius: 8px; font-family: sans-serif;">
  <pattern id="grid9" width="20" height="20" patternUnits="userSpaceOnUse">
    <path d="M 20 0 L 0 0 0 20" fill="none" stroke="#e9ecef" stroke-width="1"/>
  </pattern>
  <rect width="400" height="250" fill="url(#grid9)" />
  
  <!-- Midline Sweep -->
  <line x1="200" y1="0" x2="200" y2="250" stroke="#34495e" stroke-dasharray="6,4" stroke-width="3"/>
  <text x="210" y="25" font-weight="bold" fill="#34495e">Sweep Line (X)</text>
  
  <!-- Active Strip (d width) -->
  <rect x="150" y="0" width="100" height="250" fill="rgba(231, 76, 60, 0.15)"/>
  <text x="160" y="240" fill="#e74c3c" font-weight="bold" font-size="14">Active Strip (d)</text>
  
  <!-- Inactive Points (Behind Sweep Line) -->
  <circle cx="60" cy="100" r="4" fill="#95a5a6"/>
  <circle cx="110" cy="180" r="4" fill="#95a5a6"/>
  <!-- Unprocessed Points (Ahead of Sweep Line) -->
  <circle cx="320" cy="120" r="4" fill="#95a5a6"/>
  <circle cx="290" cy="210" r="4" fill="#95a5a6"/>
  
  <!-- Closest Pair (Inside Strip) -->
  <circle cx="175" cy="120" r="6" fill="#e74c3c"/>
  <circle cx="215" cy="150" r="6" fill="#e74c3c"/>
  <line x1="175" y1="120" x2="215" y2="150" stroke="#e74c3c" stroke-width="3"/>
  <text x="140" y="110" fill="#e74c3c" font-weight="bold">Closest Pair</text>
</svg>

```cpp
#include "../../core.h"
#include "../Primitives/Point.cpp"

// Custom comparator for the set (sorts by Y, then X)
struct CmpY {
    bool operator()(const pt& a, const pt& b) const {
        if (abs(a.y - b.y) > eps) return a.y < b.y;
        return a.x < b.x;
    }
};

// Returns the minimum distance between any two points in the array.
// O ( N log N )
double closestPairSweepLine(vector<pt>& pnts) {
    int n = pnts.size();
    if (n < 2) return 1e18;
    
    // Sort points by X coordinate
    sort(pnts.begin(), pnts.end());
    
    set<pt, CmpY> active_window;
    active_window.insert(pnts[0]);
    
    double min_dist = 1e18; // Current best 'd'
    int left_ptr = 0;
    
    for (int i = 1; i < n; i++) {
        // Remove points that are too far behind the sweep line (X distance > current best d)
        while (left_ptr < i && pnts[i].x - pnts[left_ptr].x > min_dist) {
            active_window.erase(pnts[left_ptr]);
            left_ptr++;
        }
        
        // Search strictly inside the Y-strip: [y - d, y + d]
        pt lower_bound_pt = { -1e18, pnts[i].y - min_dist };
        auto it = active_window.lower_bound(lower_bound_pt);
        
        while (it != active_window.end() && it->y - pnts[i].y <= min_dist) {
            min_dist = min(min_dist, pnts[i].distance(*it));
            it++;
        }
        
        active_window.insert(pnts[i]);
    }
    
    return min_dist;
}
```
