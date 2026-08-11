# Persistent Segment Tree — Short Notes

## [1] When to use

Use PST when you need **multiple versions** of a segment tree and want to query old versions after updates.

Especially useful for:

* Range queries on old versions.
* `k-th smallest` in a subarray.
* Count values `> x` in a subarray.

---

## [2] Functions

### `build()`

```cpp
pst.build(a);
```

Builds the **initial version**.

---

### `update()`

```cpp
pst.update(idx, val);
```

Creates a **new version** after changing `a[idx] = val`.

Old versions stay unchanged.

---

### `update_version()`

```cpp
pst.update_version(idx, val, version);
```

Updates an **existing version** instead of creating a new one.

---

### `copy()`

```cpp
pst.copy(version);
```

Creates a new version that is identical to `version`.

---

### `clone()`

```cpp
pst.clone();
```

Copies the **last version**.

---

### `query()`

```cpp
pst.query(l, r, version);
```

Returns the sum on `[l,r]` from `version`.

---

### `get()`

```cpp
pst.get(version, idx);
```

Gets the value at `idx` from `version`.

---

## `kth()`

```cpp
pst.kth(l, r, k);
```

Returns the **k-th smallest** value in `[l,r]`.

`k` is **1-based**:

```text
k = 1 → smallest
k = 2 → 2nd smallest
...
```

Condition:

```cpp
1 <= k <= r - l + 1
```

### With coordinate compression

Suppose:

```cpp
vals = sorted unique original values
```

PST stores compressed IDs.

```cpp
int id = pst.kth(l, r, k);
int answer = vals[id - 1];
```

So:

```text
kth() → compressed ID → vals[id - 1] → original value
```

**No `lower_bound` / `upper_bound` is needed after `kth()`.**

---

## `count_greater()`

```cpp
pst.count_greater(l, r, x);
```

Returns the number of values:

```text
> x
```

inside `[l,r]`.

### With coordinate compression

If `x` is an original value:

```cpp
int p = upper_bound(vals.begin(), vals.end(), x) - vals.begin();
```

Use **`upper_bound`**, NOT `lower_bound`.

Why?

```text
upper_bound(x)
    ↓
first value > x
```

Exactly what `count_greater()` needs.

Example:

```text
vals = [10, 20, 30, 40, 50]
x = 25

upper_bound(25) → 30
```

Therefore values `> 25` are:

```text
30, 40, 50
```

### Rule to remember

```text
count > x  → upper_bound(x)
count >= x → lower_bound(x)
```

---

## Coordinate Compression

```cpp
vector<int> vals = a;

sort(vals.begin(), vals.end());
vals.erase(unique(vals.begin(), vals.end()), vals.end());
```

Convert original value to compressed ID:

```cpp
int id = lower_bound(vals.begin(), vals.end(), x)
         - vals.begin() + 1;
```

Use **`lower_bound`** because you want the exact compressed position of `x`.

### Remember

```text
Original value → lower_bound() → compressed ID

kth result     → vals[id - 1]  → original value

count > x      → upper_bound(x)
count >= x     → lower_bound(x)
```

### Most important

```text
lower_bound(x) → first position >= x
upper_bound(x) → first position >  x
```
