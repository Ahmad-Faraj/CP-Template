# Geometry

## Triangle

- ### Area of a Triangle Given Three Vertices
  
  - Given three points in a 2D plane:  
    $(0,0), (X,Y), (A,B)$

  - The formula for the area of a triangle given three vertices $(x_1, y_1)$, $(x_2, y_2)$, and $(x_3, y_3)$ is:  
    $\text{Area} = \frac{1}{2} \left| x_1(y_2 - y_3) + x_2(y_3 - y_1) + x_3(y_1 - y_2) \right|$

  - Substituting $(0,0)$, $(X,Y)$, and $(A,B)$:  
    $\text{Area} = \frac{1}{2} \left| 0(Y - B) + X(B - 0) + A(0 - Y) \right|$

  - Simplifying the equation:  
    $\text{Area} = \frac{1}{2} \left| X B - Y A \right|$

## `complex<double>` as a point

`Geometry.cpp` and `Circle.cpp` use `complex<double>` for points, so the standard library
gives you most vector algebra for free. What each function does, on `c = (4, 6)`:

| Call | Result | Meaning |
| --- | --- | --- |
| `real(c)` / `imag(c)` | `4` / `6` | the x and y components |
| `a + b`, `a - b` | | vector addition and subtraction |
| `abs(c)` | `7.2111` | length, `sqrt(norm(c))` — so `abs(a - b)` is the distance |
| `norm(c)` | `52` | length **squared**, `x*x + y*y` — *not* the length |
| `arg(c)` | `0.876058` | the angle from the +x axis, `atan2(y, x)` |
| `conj(c)` | `4 - 6i` | reflection across the x axis |
| `polar(r, t)` | | the vector of length `r` at angle `t` |
| `a * polar(1.0, t)` | | `a` rotated counter-clockwise by `t` |
| `(conj(a) * b).real()` | | the dot product of `a` and `b` |
| `(conj(a) * b).imag()` | | the cross product of `a` and `b` |

Two things to watch. `norm` is the squared length, which is the most common misread here.
And the standard only specifies `complex` for `float`, `double` and `long double` — there is
no `complex<long long>`, which is why exact integer geometry lives in `Convex_Hull.cpp` and
`Polygon.cpp` on a plain struct instead.
