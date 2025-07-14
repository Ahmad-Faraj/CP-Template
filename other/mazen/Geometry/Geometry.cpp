const double PI = acos(-1.0);
class grometery
{
    static double toDegreeFromMinutes(double minutes)
    {
        return (minutes / 60);
    }

    static double toDegreeFromSeconds(double seconds)
    {
        return (seconds / 3600);
    }

    static double toRadians(double degree)
    {
        return (degree * PI / 180);
    }

    static double toDegree(double radian)
    {
        if (radian < 0)
            radian += (2 * PI);
        return (radian * 180 / PI);
    }

    static double toMinutes(double degree)
    {
        return (degree * 60);
    }

    static double toSeconds(double degree)
    {
        return (degree * 3600);
    }
    /*
    * sin(A + B) = sin(A) * cos(B) + cos(A) * sin(B)
    * cos(A + B) = cos(A) * cos(B) - sin(A) * sin(B)
    * tan(A + B) = (tan(A) + tan(B)) / (1 - tan(A) * tan(B))
    * sin(A - B) = sin(A) * cos(B) - cos(A) * sin(B)
    * cos(A - B) = cos(A) * cos(B) + sin(A) * sin(B)
    * tan(A - B) = (tan(A) - tan(B)) / (1 + tan(A) * tan(B))
    * sin(2A) = 2 * sin(A) * cos(A)
    * cos(2A) = cos^2(A) - sin^2(A)
    * tan(2A) = 2 * tan(A) / (1 - tan^2(A))
    * sin(3A) = 3 * sin(A) - 4 * sin^3(A)
    * cos(3A) = 4 * cos^3(A) - 3 * cos(A)
    * tan(3A) = (3 * tan(A) - tan^3(A)) / (1 - 3 * tan^2(A))
    * sin(4A) = 4 * sin(A) * (1 - 2 * sin^2(A))
    * cos(4A) = 1 - 8 * sin^2(A) + 8 * sin^4(A)
    * tan(4A) = 4 * tan(A) - 4 * tan^3(A) / (1 - 6 * tan^2(A) + tan^4(A))
    * sin(5A) = 5 * sin(A) - 20 * sin^3(A) + 16 * sin^5(A)
    * cos(5A) = 16 * cos^5(A) - 20 * cos^3(A) + 5 * cos(A)
    * tan(5A) = (5 * tan(A) - 10 * tan^3(A) + tan^5(A)) / (1 - 10 * tan^2(A) + 5 * tan^4(A))
    * sin(a)/a = sin(b)/b = sin(c)/c
    * cos(a) = (b^2 + c^2 - a^2) / 2bc
    * cos(b) = (c^2 + a^2 - b^2) / 2ca
    * cos(c) = (a^2 + b^2 - c^2) / 2ab
    * a*a = b*b + c*c - 2*b*c*cos(a)
    * b*b = c*c + a*a - 2*c*a*cos(b)
    * c*c = a*a + b*b - 2*a*b*cos(c)
    * sin(PI / 2 - A) = cos(A)
    * cos(PI / 2 - A) = sin(A)
    * tan(PI / 2 - A) = 1 / tan(A)
    * sin(PI - A) = sin(A)
    * cos(PI - A) = -cos(A)
    * tan(PI - A) = -tan(A)
    * sin(PI + A) = -sin(A)
    * cos(PI + A) = -cos(A)
    * tan(PI + A) = tan(A)
    * sin(2PI - A) = -sin(A)
    * cos(2PI - A) = cos(A)
    * tan(2PI - A) = tan(A)
    * sin(3PI / 2 - A) = -cos(A)
    * cos(3PI / 2 - A) = -sin(A)
    * tan(3PI / 2 - A) = -1 / tan(A)
    * 
    */

}

class point
{
public:
    double x, y;
    point(double x, double y)
    {
        this->x = x;
        this->y = y;
    }
    point operator+(point p)
    {
        return point(x + p.x, y + p.y);
    }
    point operator-(point p)
    {
        return point(x - p.x, y - p.y);
    }
    point operator*(double k)
    {
        return point(x * k, y * k);
    }
    point operator/(double k)
    {
        return point(x / k, y / k);
    }
    double operator*(point p)
    {
        return (x * p.x + y * p.y);
    }
    double operator^(point p)
    {
        return (x * p.y - y * p.x);
    }
    bool operator==(point p)
    {
        return (x == p.x && y == p.y);
    }
    bool operator!=(point p)
    {
        return (x != p.x || y != p.y);
    }
    bool operator<(point p)
    {
        return (x < p.x || (x == p.x && y < p.y));
    }
    bool operator>(point p)
    {
        return (x > p.x || (x == p.x && y > p.y));
    }
    bool operator<=(point p)
    {
        return (x < p.x || (x == p.x && y <= p.y));
    }
    bool operator>=(point p)
    {
        return (x > p.x || (x == p.x && y >= p.y));
    }
    double distance(point p)
    {
        return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
    }
    double distance()
    {
        return sqrt(x * x + y * y);
    }
    double distanceSquared(point p)
    {
        return ((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
    }
    double distanceSquared()
    {
        return (x * x + y * y);
    }
    double angle(point p)
    {
        return acos((*this * p) / (distance() * p.distance()));
    }
    double angle(point a, point b)
    {
        point p = *this;
        return acos((a - p) * (b - p) / ((a - p).distance() * (b - p).distance()));
    }
    double angle()
    {
        return atan2(y, x);
    }
    double angle(point a, point b)
    {
        return atan2((a - *this) ^ (b - *this), (a - *this) * (b - *this));
    }
    point rotate(double angle)
    {
        return point(x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle));
    }
    point rotate(point p, double angle)
    {
        return (p + (*this - p).rotate(angle));
    }
    point rotate90()
    {
        return point(-y, x);
    }
    point rotate270()
    {
        return point(y, -x);
    }
    point rotate(point p, double angle)
    {
        return (p + (*this - p).rotate(angle));
    }
    point rotate(point p, point q, double angle)
    {
        return (p + (*this - p).rotate(q - p, angle));
    }
    point reflect(point p)
    {
        return (p + p - *this);
    }
    point reflect(point p, point q)
    {
        return (p + p - *this).rotate(p, q);
    }
    point reflect(point p, point q, point r)
    {
        return (p + p - *this).rotate(p, q, r);
    }
    point perp()
    {
        return point(-y, x);
    }
    point unit()
    {
        return (*this / distance());
    }
    point normal()
    {
        return perp().unit();
    }
    point project(point p)
    {
        return p * (p * (*this) / (p * p));
    }
    point project(point p, point q)
    {
        return (p - q).project(*this - q) + q;
    }
    point intersect(point a, point b, point c, point d)
    {
        b = b - a;
        d = c - d;
        c = c - a;
        return a + b * (c ^ d) / (b ^ d);
    }

    bool isParallel(point a, point b, point c, point d)
    {
        return (a - b) ^ (c - d) == 0;
    }
    bool isCollinear(point a, point b, point c, point d)
    {
        return isParallel(a, b, c, d) && isParallel(a, c, a, d);
    }
    bool isCollinear(point a, point b, point c)
    {
        return (a - b) ^ (a - c) == 0;
    }
};

class TriangleWithPoints
{
public:
    point a, b, c;
    TriangleWithPoints(point a, point b, point c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }
    double perimeter()
    {
        return (a.distance(b) + b.distance(c) + c.distance(a));
    }
    double area()
    {
        return (fabs((a - c) ^ (b - c)) / 2);
    }
    double area(point a, point b, point c)
    {
        return (fabs((a - c) ^ (b - c)) / 2);
    }
    double area()
    {
        return (fabs((a - c) ^ (b - c)) / 2);
    }
    double area(point p)
    {
        return (fabs((a - p) ^ (b - p) + (b - p) ^ (c - p) + (c - p) ^ (a - p)) / 2);
    }
    double area(point a, point b, point c)
    {
        return (fabs((a - c) ^ (b - c)) / 2);
    }
    double area(point a, point b, point c, point p)
    {
        return (fabs((a - p) ^ (b - p) + (b - p) ^ (c - p) + (c - p) ^ (a - p)) / 2);
    }
    double area(point p, point q, point r)
    {
        return (fabs((p - r) ^ (q - r)) / 2);
    }
    double area(point p, point q, point r, point s)
    {
        return (fabs((p - s) ^ (q - s) + (q - s) ^ (r - s) + (r - s) ^ (p - s)) / 2);
    }
    bool isInside(point p)
    {
        return (fabs(area(a, b, p) + area(b, c, p) + area(c, a, p) - area()) < EPS);
    }
};
class vectors
{
public:
    double x, y;
    vectors(double x, double y)
    {
        this->x = x;
        this->y = y;
    }
    vectors operator+(vectors v)
    {
        return vectors(x + v.x, y + v.y);
    }
    vectors operator-(vectors v)
    {
        return vectors(x - v.x, y - v.y);
    }
    vectors operator*(double k)
    {
        return vectors(x * k, y * k);
    }
    vectors operator/(double k)
    {
        return vectors(x / k, y / k);
    }
    double operator*(vectors v)
    {
        return (x * v.x + y * v.y);
    }
    double operator^(vectors v)
    {
        return (x * v.y - y * v.x);
    }
    bool operator==(vectors v)
    {
        return (x == v.x && y == v.y);
    }
    bool operator!=(vectors v)
    {
        return (x != v.x || y != v.y);
    }
    bool operator<(vectors v)
    {
        return (x < v.x || (x == v.x && y < v.y));
    }
    bool operator>(vectors v)
    {
        return (x > v.x || (x == v.x && y > v.y));
    }
    bool operator<=(vectors v)
    {
        return (x < v.x || (x == v.x && y <= v.y));
    }
    bool operator>=(vectors v)
    {
        return (x > v.x || (x == v.x && y >= v.y));
    }
    double distance(vectors v)
    {
        return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
    }
    double distance()
    {
        return sqrt(x * x + y * y);
    }
    double distanceSquared(vectors v)
    {
        return ((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
    }
    double distanceSquared()
    {
        return (x * x + y * y);
    }
    double angle(vectors v)
    {
        return acos((*this * v) / (distance() * v.distance()));
    }
    double angle(vectors a, vectors b)
    {
        vectors v = *this;
        return acos((a - v) * (b - v) / ((a - v).distance() * (b - v).distance()));
    }
    double angle()
    {
        return atan2(y, x);
    }
    double angle(vectors a, vectors b)
    {
        return atan2((a - *this) ^ (b - *this), (a - *this) * (b - *this));
    }
    vectors rotate(double angle)
    {
        return vectors(x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle));
    }
    vectors rotate(vectors v, double angle)
    {
        return (v + (*this - v).rotate(angle));
    }
    vectors rotate90()
    {
        return vectors(-y, x);
    }
    vectors rotate270()
    {
        return vectors(y, -x);
    }
    vectors rotate(vectors p, double angle)
    {
        return (p + (*this - p).rotate(angle));
    }
    vectors rotate(vectors p, vectors q, double angle)
    {
        return (p + (*this - p).rotate(q - p, angle));
    }
    vectors reflect(vectors p)
    {
        return (p + p - *this);
    }
    vectors reflect(vectors p, vectors q)
    {
        return (p + p - *this).rotate(p, q);
    }
    vectors reflect(vectors p, vectors q, vectors r)
    {
        return (p + p - *this).rotate(p, q, r);
    }
    // two vectors are perpendicular if and only if their dot product is zero
    bool isPerpendicular(vectors a, vectors b)
    {
        return (a * b == 0);
    }
    // group of vectors are orthogonal if and only if every pair of vectors are perpendicular
    bool isOrthogonal(vector<vectors> v)
    {
        for (int i = 0; i < v.size(); i++)
        {
            for (int j = i + 1; j < v.size(); j++)
            {
                if (!(v[i] * v[j] == 0))
                    return false;
            }
        }
        return true;
    }
    // dot product
    double dotProduct(vectors a, vectors b)
    {
        return distance(a) * distance(b) * cos(angle(a, b));
    }
    // cross product
    double crossProduct(vectors a, vectors b)
    {
        return distance(a) * distance(b) * sin(angle(a, b));
    }
    double pallarellogramArea(vectors a, vectors b)
    {
        return distance(a) * distance(b) * sin(angle(a, b));
    }
    double triangleArea(vectors a, vectors b)
    {
        return distance(a) * distance(b) * sin(angle(a, b)) / 2;
    }
};

class TriangleWithSides
{
public:
    double a, b, c;
    TriangleWithSides(double a, double b, double c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }
    double perimeter()
    {
        return (a + b + c);
    }
    double area()
    {
        double s = perimeter() / 2;
        return (sqrt(s * (s - a) * (s - b) * (s - c)));
    }
    double area(double a, double b, double c)
    {
        double s = perimeter(a, b, c) / 2;
        return (sqrt(s * (s - a) * (s - b) * (s - c)));
    }
    double area()
    {
        double s = perimeter() / 2;
        return (sqrt(s * (s - a) * (s - b) * (s - c)));
    }
    double area(double p)
    {
        double s = perimeter(p) / 2;
        return (sqrt(s * (s - a) * (s - b) * (s - c)));
    }
    double area(double a, double b, double c, double p)
    {
        double s = perimeter(a, b, c, p) / 2;
        return (sqrt(s * (s - a) * (s - b) * (s - c)));
    }
    double area(double p, double q, double r)
    {
        double s = perimeter(p, q, r) / 2;
        return (sqrt(s * (s - a) * (s - b) * (s - c)));
    }
    double area(double p, double q, double r, double s)
    {
        double s = perimeter(p, q, r, s) / 2;
        return (sqrt(s * (s - a) * (s - b) * (s - c)));
    }
    bool isTriangle()
    {
        return (a + b > c && b + c > a && c + a > b);
    }
    bool isRight()
    {
        return (a * a + b * b == c * c || b * b + c * c == a * a || c * c + a * a == b * b);
    }
    bool isEquilateral()
    {
        return (a == b && b == c);
    }
    bool isIsosceles()
    {
        return (a == b || b == c || c == a);
    }
    bool isScalene()
    {
        return (a != b && b != c && c != a);
    }
    bool isAcute()
    {
        return (a * a + b * b > c * c && b * b + c * c > a * a && c * c + a * a > b * b);
    }
    bool isObtuse()
    {
        return (a * a + b * b < c * c || b * b + c * c < a * a || c * c + a * a < b * b);
    }
    bool isRight()
    {
        return (a * a + b * b == c * c || b * b + c * c == a * a || c * c + a * a == b * b);
    }
    bool isEquilateral(double a, double b, double c)
    {
        return (a == b && b == c);
    }
    bool isIsosceles(double a, double b, double c)
    {
        return (a == b || b == c || c == a);
    }
    bool isScalene(double a, double b, double c)
    {
        return (a != b && b != c && c != a);
    }
    bool isAcute(double a, double b, double c)
    {
        return (a * a + b * b > c * c && b * b + c * c > a * a && c * c + a * a > b * b);
    }
    bool isObtuse(double a, double b, double c)
    {
        return (a * a + b * b < c * c || b * b + c * c < a * a || c * c + a * a < b * b);
    }
    bool isRight(double a, double b, double c)
    {
        return (a * a + b * b == c * c || b * b + c * c == a * a || c * c + a * a == b * b);
    }
    bool isTriangle(double a, double b, double c)
    {
        return (a + b > c && b + c > a && c + a > b);
    }
    bool isRight(double a, double b, double c)
    {
        return (a * a + b * b == c * c || b * b + c * c == a * a || c * c + a * a == b * b);
    }
};

class Angle
{
public:
    double degree;
    Angle(double degree)
    {
        this->degree = degree;
    }
    double toDegreeFromMinutes(double minutes)
    {
        return (minutes / 60);
    }
    double toDegreeFromSeconds(double seconds)
    {
        return (seconds / 3600);
    }
    double toRadians(double degree)
    {
        return (degree * PI / 180);
    }
    double toDegree(double radian)
    {
        if (radian < 0)
            radian += (2 * PI);
        return (radian * 180 / PI);
    }
    double toMinutes(double degree)
    {
        return (degree * 60);
    }
    double toSeconds(double degree)
    {
        return (degree * 3600);
    }
    // sin(A + B) = sin(A) * cos(B) + cos(A) * sin(B)
    // cos(A + B) = cos(A) * cos(B) - sin(A) * sin(B)
    // tan(A + B) = (tan(A) + tan(B)) / (1 - tan(A) * tan(B))
    // sin(A - B) = sin(A) * cos(B) - cos(A) * sin(B)
    // cos(A - B) = cos(A) * cos(B) + sin(A) * sin(B)
    // tan(A - B) = (tan(A) - tan(B)) / (1 + tan(A) * tan(B))
    // sin(2A) = 2 * sin(A) * cos(A)
    // cos(2A) = cos^2(A) - sin^2(A)
    // tan(2A) = 2 * tan(A) / (1 - tan^2(A))
    // sin(3A) = 3 * sin(A) - 4 * sin^3(A)
    // cos(3A) = 4 * cos^3(A) - 3 * cos(A)
    // tan(3A) = (3 * tan(A) - tan^3(A)) / (1 - 3 * tan^2(A))
    // sin(4A) = 4 * sin(A) * (1 - 2 * sin^2(A))
    // cos(4A) = 1 - 8 * sin^2(A) + 8 * sin^4(A)
    // tan(4A) = 4 * tan(A) - 4 * tan^3(A) / (1 - 6 * tan^2(A) + tan^4(A))
    // sin(5A) = 5 * sin(A) - 20 * sin^3(A) + 16 * sin^5(A)
    // cos(5A) = 16 * cos^5(A) - 20 * cos^3(A) + 5 * cos(A)
    // tan(5A) = (5 * tan(A) - 10 * tan^3(A) + tan^5(A)) / (1 - 10 * tan^2(A) + 5 * tan^4(A))
};

class Triangle
{
public:
    double a, b, c;
    double A, B, C;
    Triangle(double a, double b, double c, double A, double B, double C)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        this->A = A;
        this->B = B;
        this->C = C;
    }
    double perimeter()
    {
        return (a + b + c);
    }
    double fixangle(double angle)
    {
        return angle > 1 ? 1 : (angle < -1 ? -1 : angle);
    }
    double getSide_a_bAB(double b, double A, double B)
    {
        return (b * sin(A) / sin(B));
    }
    double getAngle_A_abB(double a, double b, double B)
    {
        return (asin(fixangle(a * sin(B) / b)));
    }
    double getAngle_A_abc(double a, double b, double c)
    {
        return (acos(fixangle((b * b + c * c - a * a) / (2 * b * c))));
    }
    // right triangle
    // sin(A) = a / c
    // cos(A) = b / c
    // tan(A) = a / b

}

class Circle
{
public:
    double r;
    Circle(double r)
    {
        this->r = r;
    }
    double diameter()
    {
        return (2 * r);
    }
    double circumference()
    {
        return (2 * PI * r);
    }
    double area()
    {
        return (PI * r * r);
    }
    double arcLength(double angle)
    {
        return (angle * PI * r / 180);
    }
    double sectorArea(double angle)
    {
        return (angle * PI * r * r / 360);
    }
    double chordLength(double angle)
    {
        return (2 * r * sin(angle / 2));
    }
    double segmentArea(double angle)
    {
        return (r * r * (angle - sin(angle)) / 2);
    }
    double segmentArea(double angle, double r)
    {
        return (r * r * (angle - sin(angle)) / 2);
    }
    double segmentArea(double angle, double r, double d)
    {
        return (r * r * (angle - sin(angle)) / 2);
    }
    double segmentArea(double angle, double r, double d, double h)
    {
        return (r * r * (angle - sin(angle)) / 2);
    }
    double segmentArea(double angle, double r, double d, double h, double l)
    {
        return (r * r * (angle - sin(angle)) / 2);
    }
    double segmentArea(double angle, double r, double d, double h, double l, double a)
    {
        return (r * r * (angle - sin(angle)) / 2);
    }
}