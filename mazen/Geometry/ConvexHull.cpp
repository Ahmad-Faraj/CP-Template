#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define ld long double
#define nl "\n"
#define sz(x) (int)(x.size())
#define all(x) (x).begin(), (x).end()
#define EPS 1e-9

struct Point
{
    ll x, y;

    Point(ll x = 0, ll y = 0) : x(x), y(y) {}

    bool operator<(const Point &p) const
    {
        return x < p.x || (x == p.x && y < p.y);
    }

    bool operator==(const Point &p) const
    {
        return x == p.x && y == p.y;
    }

    Point operator-(const Point &p) const
    {
        return Point(x - p.x, y - p.y);
    }
};

// Returns 2 times the signed area of the triangle formed by points p, q, and r
ll cross(const Point &O, const Point &A, const Point &B)
{
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Returns true if points p, q, r make a counter-clockwise turn
bool ccw(const Point &p, const Point &q, const Point &r)
{
    return cross(p, q, r) > 0;
}

// Returns distance between two points
ld dist(const Point &p1, const Point &p2)
{
    return hypot(p1.x - p2.x, p1.y - p2.y);
}

// Computes the convex hull of a set of points
// Returns points in counter-clockwise order
vector<Point> convexHull(vector<Point> points)
{
    int n = sz(points);
    if (n <= 2)
        return points;

    // Find the bottommost point (and leftmost if tied)
    int basePoint = 0;
    for (int i = 1; i < n; i++)
    {
        if (points[i].y < points[basePoint].y ||
            (points[i].y == points[basePoint].y && points[i].x < points[basePoint].x))
        {
            basePoint = i;
        }
    }

    // Put the base point at position 0
    swap(points[0], points[basePoint]);

    // Sort points by polar angle with respect to base point
    Point p0 = points[0];
    sort(points.begin() + 1, points.end(), [&p0](const Point &p1, const Point &p2)
         {
        ll c = cross(p0, p1, p2);
        if (c != 0) return c > 0;
        return dist(p0, p1) < dist(p0, p2); });

    // Build convex hull
    vector<Point> hull;
    hull.push_back(points[0]);
    hull.push_back(points[1]);

    for (int i = 2; i < n; i++)
    {
        while (sz(hull) >= 2 && !ccw(hull[sz(hull) - 2], hull[sz(hull) - 1], points[i]))
        {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    return hull;
}

// Example usage
void solve()
{
    int n;
    cin >> n;
    vector<Point> points(n);
    for (auto &p : points)
    {
        cin >> p.x >> p.y;
    }

    vector<Point> hull = convexHull(points);

    // Output hull points
    cout << sz(hull) << nl;
    for (const auto &p : hull)
    {
        cout << p.x << " " << p.y << nl;
    }

    // Calculate perimeter (optional)
    ld perimeter = 0;
    for (int i = 0; i < sz(hull); i++)
    {
        perimeter += dist(hull[i], hull[(i + 1) % sz(hull)]);
    }
    cout << fixed << setprecision(9) << perimeter << nl;
}

signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int tc = 1;
    // cin >> tc;
    while (tc--)
    {
        solve();
    }
    return 0;
}