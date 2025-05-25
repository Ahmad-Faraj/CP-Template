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
