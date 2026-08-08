# Computational Geometry Masterclass: Problem Patterns & Heuristics

This document serves as the theoretical and pattern-recognition backbone for Competitive Programming Geometry, ranging from Candidate Master (1900) to International Grandmaster (2600+).

## 1. Purple/Master Level Patterns (1900-2300)

### 1.1 Vector Operations & Primitives
**Concept:** Relying exclusively on Cross Products and Dot Products to avoid precision loss. Division and trigonometric functions (`atan2`, `acos`) introduce floating-point inaccuracies.
* **Cross Product ($A \times B$):** Determines orientation. $A \times B > 0$ means $B$ is strictly counter-clockwise (Left) to $A$. $A \times B = 0$ means collinear. Used heavily for checking polygon convexity and segment intersections.
* **Dot Product ($A \cdot B$):** Determines angle properties. $A \cdot B = 0$ means orthogonal (perpendicular). $A \cdot B < 0$ means the angle is obtuse. Used for projecting points onto lines or checking if a point lies exactly on a segment bounded by its endpoints.
* **Problem Signature:** You are asked to process angles, sort points angularly around a center, or check spatial properties on a grid.
* **Complexity Bounds:** $O(1)$ memory, $O(N \log N)$ for radial sort.
* **Heuristic:** Always use integer coordinates `long long` for cross/dot products when the grid allows it.

### 1.2 Convex Hull Variations
**Concept:** Finding the smallest convex envelope around a set of points.
* **Monotone Chain Algorithm:** Preferred over Graham Scan. It lexicographically sorts points by $X$ (then $Y$) and computes the upper and lower boundaries in two linear sweeps.
* **Handling Collinear Points:** Depending on the problem, you may need to preserve points that lie perfectly flat on the hull boundary. This requires changing the cross-product threshold from $\le 0$ to $< 0$.
* **Problem Signature:** Queries asking to enclose elements, find the maximum area polygon from a point set, or determine the maximum distance between two points (Diameter).
* **Complexity Bounds:** $O(N \log N)$ time (dominated by sorting), $O(N)$ memory.

### 1.3 Sweep Line Fundamentals
**Concept:** A vertical line sweeps across the 2D plane from left to right (or angular sweep around a point), maintaining an active state of intersected elements using a balanced binary search tree (`std::set`).
* **Segment Intersections (Bentley-Ottmann):** Finds all intersection points among $N$ segments.
* **Area of Union of Rectangles:** Sweeping vertical edges while a Segment Tree maintains the active horizontal lengths.
* **Problem Signature:** "Given $N \le 10^5$ rectangles, find the total covered area" or "Find if any two segments intersect among $10^5$ segments."
* **Complexity Bounds:** $O(N \log N)$ time. Memory is strictly $O(N)$ for the active data structure.

### 1.4 Polygon Basics
**Concept:** Fundamental querying of closed boundaries.
* **Point in Polygon (Ray Casting):** Shoot an infinite ray rightwards. If it crosses an odd number of edges, the point is inside. $O(N)$ time.
* **Shoelace Formula:** Computes the exact area of any non-self-intersecting polygon. $O(N)$ time.
* **Pick's Theorem:** $Area = Interior + Boundary/2 - 1$. Used exclusively for lattice point grids (integer coordinates).
* **Problem Signature:** Counting the number of integer coordinate trees inside a designated boundary area.
* **Complexity Bounds:** $O(N)$ time, $O(1)$ auxiliary memory.

### 1.5 Precision Management
**Concept:** Floating-point comparison is the silent killer in CP geometry.
* **The Epsilon ($\epsilon$):** Define `const double eps = 1e-9;`.
* **Equality:** Never use `a == b`. Use `abs(a - b) < eps`.
* **Heuristic:** Postpone division, `sqrt()`, and trigonometry to the absolute last step of a calculation. Maintain squared distances instead of actual distances whenever comparing lengths.

---

## 2. Grandmaster Level Patterns (2400-2600+)

### 2.1 Complex Convexity
* **Minkowski Sum:** The geometric addition of two convex polygons $P$ and $Q$. The resulting shape contains all possible vector sums $p+q$. Solved in $O(N+M)$ by sequentially merging edges sorted by angle.
* **Rotating Calipers:** Maintaining two parallel tangent lines sweeping around a convex hull to find the maximum diameter (farthest pair of points) or the minimum bounding rectangle in $O(N)$.
* **Problem Signature:** "Given two convex polygons, can you translate one to fit entirely inside the other without rotating?" or "Find the maximum distance between any two points in $10^5$ elements."
* **Complexity Bounds:** Strict $O(N)$ assuming the polygon/hull is already sorted/constructed.

### 2.2 Half-Plane Intersection
**Concept:** Finding the feasible region formed by $N$ intersecting linear constraints.
* **Algorithm:** Sort all directed lines by angle. Maintain a `deque` of lines that form the boundary of the intersection. Process in $O(N \log N)$.
* **Duality / Algebraic Disguise:** Many problems present themselves as a system of linear inequalities (e.g., $Ax + By \le C$). In a 2D plane, each inequality is simply a Half-Plane.
* **Problem Signature:** Optimization problems asking to maximize a linear function over a set of linear constraints (Linear Programming), or finding the kernel of a polygon.
* **Complexity Bounds:** $O(N \log N)$ time, $O(N)$ memory for the deque.

### 2.3 Circles & Arcs
* **Circle Intersections & Areas:** Requires trigonometry. Circle-circle area uses the Law of Cosines to calculate sector areas minus triangle areas.
* **Inversion Geometry:** A transformation that maps points inside a circle to the outside and vice-versa. Lines that don't pass through the center become circles passing through the center. Used to simplify notoriously hard problems involving multiple mutually tangent circles.
* **Minimum Enclosing Circle (Welzl's):** A randomized recursive algorithm that finds the absolute smallest circle encompassing $N$ points in $O(N)$ expected time.
* **Problem Signature:** "Place a circular radio tower to cover all given targets while minimizing transmission range."
* **Complexity Bounds:** Area intersections are $O(1)$ math operations. Welzl's is expected $O(N)$, worst case $O(N^3)$ if un-shuffled.

### 2.4 Planar Graphs & Subdivisions
* **Euler's Formula:** $V - E + F = 1 + C$. Crucial for answering queries about how many distinct regions are created by overlapping geometric primitives.
* **Point Location (Kirkpatrick / Persistent Segment Trees):** Querying which polygonal region a point falls into.
* **Problem Signature:** "Given $N$ intersecting circles or lines, how many distinct enclosed regions are formed?"
* **Complexity Bounds:** Euler components can be calculated in $O(E \log E)$ or $O(V \log V)$ using sweep lines to find intersections.

### 2.5 Complex Sweep Line: Voronoi & Delaunay
**Concept:** 
* **Voronoi Diagram:** Partitions the plane into regions based on the distance to a specific set of seed points.
* **Delaunay Triangulation:** The dual graph of the Voronoi diagram. It connects points such that no point is inside the circumcircle of any triangle.
* **Problem Signature:** "Given a set of post offices, answer $10^5$ queries asking which post office is strictly the closest to coordinate $(X, Y)$."
* **Complexity Bounds:** Fortune's Sweep Line Algorithm runs in $O(N \log N)$ time. Delaunay triangulations are typically required for minimizing angles.

### 2.6 3D Geometry
**Concept:** Moving primitives into 3 dimensions.
* **Vectors:** Cross product now returns a vector orthogonal to the plane of the two input vectors.
* **Equations:** Planes are defined by a normal vector and a point. Spheres replace circles. 
* **3D Convex Hull:** Constructed using an $O(N^2)$ incremental algorithm to form a polyhedron wrapper.
* **Problem Signature:** Usually direct and explicitly 3-dimensional. Involves computing intersection volumes or verifying coplanarity.
* **Complexity Bounds:** $O(1)$ for primitives, $O(N^2)$ or $O(N \log N)$ for 3D structures. Memory management must be highly efficient as coordinates blow up quickly.
