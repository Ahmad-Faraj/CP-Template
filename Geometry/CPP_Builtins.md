# C++ Standard Library: Geometry Built-ins

C++ natively provides a vast array of mathematical and geometric operations via `<cmath>`, `<complex>`, and `<numeric>`. Utilizing these correctly prevents precision errors, undefined behaviors, and excessive custom implementations.

## 1. The `<complex>` Library (2D Points)
The `<complex>` library provides a highly optimized 2D point structure.

```cpp
#include <complex>
using namespace std;

typedef complex<double> point;
#define x real()
#define y imag()

point A(2.0, 3.0);
point B(5.0, 7.0);

// Vector Addition and Subtraction
point C = A + B; 
point D = B - A;
```

### 1.1 Distances and Magnitudes
- **`abs(z)`**: Returns the Euclidean distance (hypotenuse) $\sqrt{x^2 + y^2}$.
- **`norm(z)`**: Returns the squared magnitude $x^2 + y^2$. Faster than `abs()` as it avoids the square root.

### 1.2 Angles and Polar Coordinates
- **`arg(z)`**: Returns the polar angle of the point in radians, strictly in the range $[-\pi, \pi]$.
- **`polar(r, theta)`**: Constructs a point given a magnitude `r` and an angle `theta`.

### 1.3 Geometric Transformations via Complex Arithmetic
- **`conj(z)`**: Returns the complex conjugate $(x, -y)$. Geometrically, this is a reflection across the X-axis.
- **Rotation**: Multiplying two complex numbers adds their angles and multiplies their magnitudes. To rotate point $A$ by $\theta$ radians counter-clockwise around the origin:
  ```cpp
  point rotated_A = A * polar(1.0, theta);
  // Alternatively using Euler's formula: A * exp(point(0, theta))
  ```
- **Scaling and Rotation**: `A * polar(S, theta)` scales the vector by $S$ and rotates it by $\theta$.
- **Angle between two vectors**: `arg(B / A)` returns the directed angle from vector $A$ to vector $B$.

### 1.4 Dot and Cross Products via `conj()`
By multiplying the conjugate of $A$ with $B$, you compute both products simultaneously:
- $(A.x - i A.y) \times (B.x + i B.y) = (A.x B.x + A.y B.y) + i (A.x B.y - A.y B.x)$
```cpp
double dotProduct(point a, point b) { return (conj(a) * b).real(); }
double crossProduct(point a, point b) { return (conj(a) * b).imag(); }
```

## 2. The `<cmath>` Library

### 2.1 Trigonometry Fundamentals
- **`acos(-1.0)`**: The standard and most precise way to define $\pi$.
- **`sin(x), cos(x), tan(x)`**: Standard trigonometric functions (input in radians).
- **`asin(x), acos(x)`**: Inverse trigonometry. *Warning: Input must be strictly within $[-1.0, 1.0]$ or it returns `NaN`.*
- **`atan2(y, x)`**: Computes $\arctan(y/x)$ safely. It handles the quadrant correctly and avoids division by zero if $x = 0$. Always prefer `atan2` over `atan`.

### 2.2 Distances
- **`hypot(x, y)`**: Computes $\sqrt{x^2 + y^2}$ safely, preventing intermediate overflow if $x^2 + y^2$ exceeds the bounds of `double`.
- **`hypot(x, y, z)`** *(C++17)*: Computes the 3D distance $\sqrt{x^2 + y^2 + z^2}$ safely.

### 2.3 Angle Wrapping and Floating Modulo
- **`fmod(x, y)`**: Returns the floating-point remainder of $x / y$. Useful for wrapping angles.
  ```cpp
  // Wraps an angle to [0, 2*PI)
  double wrap_angle(double theta) {
      double res = fmod(theta, 2 * PI);
      if (res < 0) res += 2 * PI;
      return res;
  }
  ```
- **`remainder(x, y)`**: Similar to `fmod`, but rounds to the nearest integer rather than truncating towards zero.

### 2.4 Linear Interpolation (C++20)
- **`std::lerp(a, b, t)`**: Computes $a + t(b - a)$. In 1D geometry, this finds a point at parameterized distance $t$ between $a$ and $b$.
  ```cpp
  // Find the midpoint of 1D coordinates
  double mid = lerp(x1, x2, 0.5);
  ```

## 3. The `<algorithm>` & `<numeric>` Libraries (Geometric Contexts)

### 3.1 Precision Safety (C++17)
- **`std::clamp(val, min, max)`**: Crucial for protecting `asin` and `acos` from floating-point inaccuracies. If a dot-product calculation yields `1.0000000000000002`, passing it to `acos` yields `NaN`.
  ```cpp
  double angle = acos(clamp(dot_val, -1.0, 1.0));
  ```

### 3.2 Lattice Points (C++17)
- **`std::gcd(a, b)`**: Essential for Pick's Theorem and Ray marching on integer grids. The number of strictly interior integer points on a segment from $(x_1, y_1)$ to $(x_2, y_2)$ is exactly `std::gcd(abs(x1 - x2), abs(y1 - y2)) + 1`. (Available in `<numeric>`).
