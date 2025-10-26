#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define ld long double
#define pt complex<double>
#define X real()
#define Y imag()

#define angle(a) (atan2((a).imag(), (a).real()))
#define vec(a, b) ((b) - (a))
#define length(a) (hypot((a).imag(), (a).real()))
#define normalize(a) (a) / length(a) // unit vector
#define same(a, b) (dot(vec(a, b), vec(a, b))) < eps
// angle in radians = atan2(y, x) = arg(pt)
// norm -> length squared
// abs -> length

const double PI = acos(-1);
const double eps = 1e-12;
const int INF = INT_MAX;

// TODO : Transformations

// ---------------------------------------------------------
//                          Rules
// ---------------------------------------------------------
// Law of Sin:  a/sin(A) = b/sin(B) = c/sin(C) -> AAS, ASA, SSA
// Law of Cos:  c^2 = a^2 + b^2 - 2ab*cos(C)   -> SAS, SSS

ld cosRule(ld a, ld b, ld c) {
    ld res = (b * b + c * c - a * a) / (2 * b * c);
    if (fabs(res - 1) < eps) res = 1;
    if (fabs(res + 1) < eps) res = -1;
    return acos(res);
}

double get_a_bAB(double b, double A, double B) {
    return (sin(A) * b) / sin(B);
}

double get_A_abB(double a, double b, double B) {
    return asin(fixAngle((a * sin(B)) / b));
}

double get_A_abc(double a, double b, double c) {
    return acos(fixAngle((b * b + c * c - a * a) / (2 * b * c)));
}

// ---------------------------------------------------------
//                         Systems
// ---------------------------------------------------------

pair<double, double> cartesian_to_polar(pt p) {
    return {abs(p), arg(p)}; // (r, θ)
}

pt polar_to_cartesian(double r, double theta) {
    return polar(r, theta); // (x, y)
}

// ---------------------------------------------------------
//                       Utilities
// ---------------------------------------------------------

ld fixAngle(ld a) {
    return (a > 1 ? 1 : ((a < -1) ? -1 : a));
}

int dcmp(ld a, ld b) {
    return fabs(a - b) <= eps ? 0 : a < b ? -1 : 1;
}

ld dot(pt a, pt b) {
    // return a.X * b.X + a.Y * b.Y​
    return ((conj(a) * (b)).real());
}

ld cross(pt a, pt b) {
    // return a.X * b.Y - b.X * a.Y​
    return ((conj(a) * (b)).imag());
}

pt mid(const pt &a, const pt &b) {
    return {(a.X + b.X) / 2.0L, (a.Y + b.Y) / 2.0L};
}

// ---------------------------------------------------------
//                         Angles
// ---------------------------------------------------------

double deg_to_rad(double deg) {
    return (deg * PI / 180.0);
}

double rad_to_deg(double rad) {
    if (rad < 0) rad += 2 * PI;
    return rad * 180.0 / PI;
}

ld angleO(pt a, pt O, pt b) /// angle(aOb)
{
    pt v1 = (a - O), v2 = (b - O);
    return acos(fixAngle(dot(v1, v2) / (abs(v1) * abs(v2))));
}

pt perp1(pt a) { // rotate 90° ccw
    return {-a.Y, a.X};
}

pt perp2(pt a) { // rotate 90° cw
    return {a.Y, -a.X};
}

ld orient(pt a, pt b, pt c) // CCW test
{
    // 1 if positive (C is to the left of AB vector) CCW
    // 0 if Zero (C is on the same direction as AB vector) Collinear
    // -1 if negative (C is to the right of AB vector) CW
    return cross(b - a, c - a);
}

int ccw(pt a, pt b, pt c) {
    pt v1(a - b), v2(c - a);
    if (cross(v1, v2) > +eps) return +1;
    if (cross(v1, v2) < -eps) return -1;
    if (v1.X * v2.X < -eps || v1.Y * v2.Y < -eps) return -1;
    if (norm(v1) < norm(v2) - eps) return +1;
    return 0;
}

complex<int> cntr;
ll cp(complex<int> a, complex<int> b) {
    return 1LL * a.real() * b.imag() - 1LL * a.imag() * b.real();
}

bool sort_ccw(complex<int> a, complex<int> b) {
    a -= cntr;
    b -= cntr;
    if (cp(a, b) == 0) {
        if (a.real() != b.real()) return a.real() < b.real();
        return a.imag() > b.imag();
    }
    return cp(a, b) > 0;
}

// ---------------------------------------------------------
//                    Points & Vectors
// ---------------------------------------------------------

bool isCollinear(pt a, pt b, pt c) {
    return dcmp(cross(b - a, c - a), 0) == 0;
}

pt rotateAbout(const pt &p, const pt &about, double radians) {
    // +rad -> ccw | -rad -> cw
    return (p - about) * exp(pt(0, radians)) + about;
}

pt reflect(const pt &p, const pt &about1, const pt &about2) {
    pt z = p - about1;
    pt w = about2 - about1;
    return conj(z / w) * w + about1;
}

bool inAngle(pt a, pt b, pt c, pt p) // point b in angle aBc
{
    if (orient(a, b, c) < 0) swap(b, c);
    return orient(a, b, p) >= 0 && orient(a, c, p) <= 0;
}

bool poinOnRay(pt a, pt b, pt c) {
    if (!isCollinear(a, b, c)) return false;
    return dcmp(dot(b - a, c - a), 0) != -1;
}

bool pointOnSegment(pt a, pt b, pt c) {
    return dcmp(abs(b - a), (abs(c - a) + abs(b - c))) == 0;
}

ld pointLineDist(const pt &a, const pt &b, const pt &p) {
    if (same(a, b)) return hypot(a.X - p.X, a.Y - p.Y);
    // dist >= 0 := Point above line
    // dist < 0 := Point Below line
    return fabs(cross(vec(a, b), vec(a, p)) / length(vec(a, b)));
}

ld pointSegmentDist(const pt &a, const pt &b, const pt &p) {
    double len = abs(a - b) * abs(a - b);
    if (dcmp(len, 0) == 0) return abs(p - a);
    double t = max((ld)0, min((ld)1, dot(p - a, b - a) / len));
    pt x = a + t * (b - a);
    return abs(x - p);
}

// ---------------------------------------------------------
//                    Lines & Segments
// ---------------------------------------------------------

pt lineLineIntersection(pt A, pt B, pt C, pt D) {
    // Line AB represented as a1x + b1y = c1
    ld a1 = B.Y - A.Y;
    ld b1 = A.X - B.X;
    ld c1 = A.X * B.Y - B.X * A.Y;
    // Line CD represented as a2x + b2y = c2
    ld a2 = D.Y - C.Y;
    ld b2 = C.X - D.X;
    ld c2 = C.X * D.Y - D.X * C.Y;
    ld determinant = a1 * b2 - a2 * b1;
    if (determinant == 0) {
        // Same line aka Rakbin fo2 b3d
        if (isCollinear(A, B, C) && isCollinear(A, B, D)) return pt(INF, INF);
        // The lines are parallel.
        return pt(-INF, -INF);
    } else {
        ld x = (b2 * c1 - b1 * c2) / determinant;
        ld y = (a1 * c2 - a2 * c1) / determinant;
        return pt(x, y);
    }
}

bool segmentSegmentIntersection(pt a, pt b, pt c, pt d, pt &out) {
    double oa = orient(c, d, a), ob = orient(c, d, b), oc = orient(a, b, c), od = orient(a, b, d);
    if (oa * ob < 0 && oc * od < 0) {
        out = (a * ob - b * oa) / (ob - oa);
        return true;
    }
    return false;
}

pt SegmentLineIntersection(pt p, pt q, pt A, pt B) {
    double a = B.Y - A.Y, b = A.X - B.X, c = B.X * A.Y - A.X * B.Y;
    double u = fabs(a * p.X + b * p.Y + c);
    double v = fabs(a * q.X + b * q.Y + c);
    if (fabs(u + v) < eps) return p;
    return {(p.X * v + q.X * u) / (u + v), (p.Y * v + q.Y * u) / (u + v)};
}

bool checkEqualLines(pair<pair<int, int>, pair<int, int>> x, pair<pair<int, int>, pair<int, int>> y) {
    if (x == y) return true;
    // x1 y1 x2 y2 x3 y3 x4 y4
    // p1 = (x.x1, x.y1), p2 = (x.x2, x.y2), p3 = (y.x1, y.y1), p4 =(y.x2, y.y2)
    return (x.x1 * (x.y2 - y.y1) + x.x2 * (y.y1 - x.y1) + y.x1 * (x.y1 - x.y2) == 0 &&
            x.x1 * (x.y2 - y.y2) + x.x2 * (y.y2 - x.y1) + y.x2 * (x.y1 - x.y2) == 0);
}

int lineLatticePointsCount(int x1, int y1, int x2, int y2) {
    return abs(__gcd(x1 - x2, y1 - y2)) + 1;
}

pair<pt, pt> generate_points_on_segment(ld A, ld B, ld C, ld t1 = 0.0L, ld t2 = 0.5L) {
    pt p0, p_dir; // Ax + By + C = 0 || t1 / t2 parameteric values
    if (fabsl(B) > eps) {
        p0 = {0.0L, -C / B};
        p_dir = {1.0L, -(A * 1.0L + C) / B};
    } else {
        p0 = {-C / A, 0.0L};
        p_dir = {-C / A, 1.0L};
    }
    return {p0 + (p_dir - p0) * (double)t1, p0 + (p_dir - p0) * (double)t2};
}

// ---------------------------------------------------------
//                        Triangles
// ---------------------------------------------------------

long double triangleAreaBH(long double b, long double h) {
    return b * h / 2;
}

long double triangleArea2sidesAngle(long double a, long double b, long double t) {
    return fabsl(a * b * sinl(t) / 2);
}

long double triangleArea2anglesSide(long double t1, long double t2, long double s) {
    return fabsl(s * s * sinl(t1) * sinl(t2) / (2 * sinl(t1 + t2)));
}

long double triangleArea3sides(long double a, long double b, long double c) {
    long double s = (a + b + c) / 2;
    return sqrtl(s * (s - a) * (s - b) * (s - c));
}

long double triangleArea3points(const pt &a, const pt &b, const pt &c) {
    return fabsl(cross(a, b) + cross(b, c) + cross(c, a)) / 2;
}

// ---------------------------------------------------------
//                         Circles
// ---------------------------------------------------------

int circleLineIntersection(const pt &p0, const pt &p1, const pt &cen, long double rad, pt &r1, pt &r2) {
    if (same(p0, p1)) {
        if (fabs(norm(vec(p0, cen)) - (rad * rad)) < eps) {
            r1 = r2 = p0;
            return 1;
        }
        return 0;
    }
    double a, b, c, t1, t2;
    a = dot(p1 - p0, p1 - p0);
    b = 2 * dot(p1 - p0, p0 - cen);
    c = dot(p0 - cen, p0 - cen) - rad * rad;
    double det = b * b - 4 * a * c;
    int res;
    if (fabs(det) < eps)
        det = 0, res = 1;
    else if (det < 0)
        res = 0;
    else
        res = 2;
    det = sqrt(det);
    t1 = (-b + det) / (2 * a);
    t2 = (-b - det) / (2 * a);
    r1 = p0 + t1 * (p1 - p0);
    r2 = p0 + t2 * (p1 - p0);
    return res;
}

int circleCircleIntersection(pt c1, double r1, pt c2, double r2, pt &res1, pt &res2) {
    if (same(c1, c2) && fabs(r1 - r2) < eps) {
        res1 = res2 = c1;
        return fabs(r1) < eps ? 1 : INF;
    }
    double len = abs(c2 - c1);
    if (fabs(len - (r1 + r2)) < eps || fabs(fabs(r1 - r2) - len) < eps) {
        pt d, c;
        double r;
        if (r1 > r2)
            d = c2 - c1, c = c1, r = r1;
        else
            d = c1 - c2, c = c2, r = r2;
        res1 = res2 = (d / abs(d)) * r + c;
        return 1;
    }
    if (len > r1 + r2 || len < fabs(r1 - r2)) return 0;
    double a = cosRule(r2, r1, len);
    pt c1c2 = ((c2 - c1) / abs(c2 - c1)) * r1;
    res1 = rotateAbout(c1c2, pt(0, 0), a) + c1;
    res2 = rotateAbout(c1c2, pt(0, 0), -a) + c1;
    return 2;
}

ld circle_intersection_area(pt o1, ld r1, pt o2, ld r2) {
    ld dx = (ld)(o1.real() - o2.real());
    ld dy = (ld)(o1.imag() - o2.imag());
    ld d = sqrtl(dx * dx + dy * dy);

    const ld EPS = 1e-15L;
    const ld PI = acosl(-1.0L);

    if (d >= r1 + r2 - EPS) return 0.0L;
    if (d <= fabsl(r1 - r2) + EPS) {
        ld rmin = min(r1, r2);
        return PI * rmin * rmin;
    }

    ld cos1 = (d * d + r1 * r1 - r2 * r2) / (2.0L * d * r1);
    ld cos2 = (d * d + r2 * r2 - r1 * r1) / (2.0L * d * r2);
    if (cos1 > 1.0L) cos1 = 1.0L;
    if (cos1 < -1.0L) cos1 = -1.0L;
    if (cos2 > 1.0L) cos2 = 1.0L;
    if (cos2 < -1.0L) cos2 = -1.0L;

    ld alpha = 2.0L * acosl(cos1);
    ld beta = 2.0L * acosl(cos2);

    ld area = 0.5L * r1 * r1 * (alpha - sinl(alpha)) + 0.5L * r2 * r2 * (beta - sinl(beta));
    return area;
}

pair<ld, pt> circumCircle(pt a, pt b, pt c) {
    double A1 = 2 * (b.X - a.X);
    double B1 = 2 * (b.Y - a.Y);
    double C1 = b.X * b.X + b.Y * b.Y - a.X * a.X - a.Y * a.Y;
    double A2 = 2 * (c.X - b.X);
    double B2 = 2 * (c.Y - b.Y);
    double C2 = c.X * c.X + c.Y * c.Y - b.X * b.X - b.Y * b.Y;
    double x = ((C1 * B2) - (C2 * B1)) / ((A1 * B2) - (A2 * B1));
    double y = ((A1 * C2) - (A2 * C1)) / ((A1 * B2) - (A2 * B1));
    pt center(x, y);
    return make_pair(norm((center - A1)), center);
}

// Minimum Enclosing Circle
pt pnts[100001], r[3], cen;
ld rad;
int ps, rs; // ps = n, rs = 0, initially
// Pre-condition
// random_shuffle(pnts, pnts+ps);
void MEC() {
    if (ps == 0 && rs == 2) {
        cen = (r[0] + r[1]) / 2.0; // (ld)2.0 casting
        rad = length(r[0] - cen);
    } else if (rs == 3) {
        pair<ld, pt> p = circumCircle(r[0], r[1], r[2]);
        cen = p.second;
        rad = p.first;
    } else if (ps == 0) {
        cen = r[0]; // sometime be garbage, but will not affect
        rad = 0;
    } else {
        ps--;
        MEC();
        if (length(pnts[ps] - cen) > rad) {
            r[rs++] = pnts[ps];
            MEC();
            rs--;
        }
        ps++;
    }
}

// ---------------------------------------------------------
//                        Polygons
// ---------------------------------------------------------

ld polygonArea(vector<pt> &v) {
    // can be implemented using pairs though to save time
    // cross product = (x1 * y2) - (x2 * y1)
    ld area = 0;
    int lst = v.size() - 1;
    for (int i = 0; i < v.size(); i++) area += cross(v[lst], v[i]), lst = i;
    return fabs(area / 2.0);
}

bool isInsidePolygon(vector<pt> &v, pt p) {
    int wn = 0;
    int cur = v.size() - 1;
    for (int nxt = 0; nxt < v.size(); nxt++) {
        if (pointOnSegment(v[cur], v[nxt], p)) return false;
        if (v[cur].imag() <= p.imag()) // up
            wn += (v[nxt].imag() > p.imag() && cross(v[nxt] - v[cur], p - v[cur]) > eps);
        else // down
            wn -= (v[nxt].imag() <= p.imag() && cross(v[nxt] - v[cur], p - v[cur]) < -eps);
        cur = nxt;
    }
    return wn;
}

pt polygonCentroid(vector<pt> &v) {
    // can be implemented using pairs though to save time
    // cross product = (x1*y2) - (x2*y1)
    int lst = v.size() - 1;
    ld area = 0, x = 0, y = 0, c;
    for (int i = 0; i < v.size(); i++) {
        c = cross(v[lst], v[i]), area += c;
        x += (v[i].real() + v[lst].real()) * c;
        y += (v[i].imag() + v[lst].imag()) * c;
        lst = i;
    }
    if (area < eps)                     // line
        return (v[0] + v.back()) * 0.5; // cast (ld) to 0.5;
    area /= 2;
    x /= 6.0 * area, y /= 6.0 * area;
    // extra precision
    if (x < eps) x = 0;
    if (y < eps) y = 0;
    return pt(x, y);
}

vector<pt> cut_polygon(pt A, pt B, const vector<pt> &Q) {
    vector<pt> p;
    for (int i = 0; i < (int)Q.size(); ++i) {
        double left1 = cross(B - A, Q[i] - A), left2 = 0;
        if (i != (int)Q.size() - 1) left2 = cross(B - A, Q[i + 1] - A);
        if (left1 > -eps) p.push_back(Q[i]);
        if (left1 * left2 < -eps) p.push_back(SegmentLineIntersection(Q[i], Q[i + 1], A, B));
    }
    if (!p.empty() && abs(p.back() - p.front()) > eps) p.push_back(p.front());
    return p;
}

ll picksTheorm(vector<pt> &p) {
    ld area = 0;
    int bound = 0;
    p.push_back(p[0]);
    for (int i = 0; i < p.size() - 1; i++) {
        int j = i + 1;
        area += cross(p[i], p[j]);
        pt v = p[j] - p[i];
        bound += abs(__gcd((int)v.real(), (int)v.imag()));
    }
    p.pop_back();
    area /= 2.0;
    area = fabs(area);
    return round(area - bound / 2 + 1);
}

// ---------------------------------------------------------
//                        Convex
// ---------------------------------------------------------

bool isConvex(vector<pt> &p) {
    int n = p.size();
    p.push_back(p[0]);
    p.push_back(p[1]);
    ld cp;
    bool pos = 0, neg = 0;
    for (int i = 0; i < n; i++) {
        cp = cross((p[i + 1] - p[i]), (p[i + 2] - p[i]));
        pos |= (cp > eps), neg |= (cp < -eps);
    }
    p.pop_back();
    p.pop_back();
    return pos ^ neg; /// duplicate points will fail this function
}

void convexPolygonIntersect(const vector<pt> &p, const vector<pt> &q, vector<pt> &res) {
    res = q;
    for (int i = 0; i < p.size(); ++i) {
        int j = (i + 1) % p.size();
        vector<pt> temp;
        temp = cut_polygon(p[i], p[j], res);
        res = temp;
        if (res.empty()) return;
    }
}

void voronoi(const vector<pt> &pnts, const vector<pt> &rect, vector<vector<pt>> &res) {
    res.clear();
    for (int i = 0; i < pnts.size(); ++i) {
        res.push_back(rect);
        for (int j = 0; j < pnts.size(); ++j) {
            if (j == i) continue;
            pt p = perp1(vec(pnts[i], pnts[j]));
            pt m = mid(pnts[i], pnts[j]);
            vector<pt> temp;
            temp = cut_polygon(m, {m.X + p.X, m.Y + p.Y}, temp);
            res.back() = temp;
        }
    }
}

// ---------------------------------------------------------
//                     Convex Hull
// ---------------------------------------------------------
