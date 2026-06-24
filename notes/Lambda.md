# C++17 Lambdas

* `[]` No capture | `[&]` By reference | `[=]` By value

```cpp
// 1. Non-Recursive

auto add = [](int a, int b) { return a + b; };
add(5, 3); 
```

```cpp
// 2. Recursive (auto&& | function<r_type(param,param)>)

auto gcd = [&](int a, int b, auto&& gcd) -> int {
    return b == 0 ? a : gcd(b, a % b, gcd);
};
gcd(20, 30, gcd); 
```
