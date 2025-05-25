#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
#define ull unsigned long long
#define ld long double
#define int long long
#define nl "\n"
#define oo 1e9 + 1
#define OO 1e18 + 1
#define sp ' '
#define sz(x) (int)(x.size())
#define MOD 1000000007
#define fixed(n) fixed << setprecision(n)
#define sub_mod(a, b, m) ((((a) % m) - ((b) % m) + m) % m)
#define add_mod(a, b, m) ((((a) % m) + ((b) % m)) % m)
#define mult_mod(a, b, m) ((((a) % m) * ((b) % m)) % m)
#define EPS 1e-9
#define PI acos(-1)
using namespace __gnu_pbds;
using namespace std;

// linear diophantine equation
// ax + by = c
// a * x + b * y = gcd(a, b) -> bezout's identity
// if c % gcd(a, b) == 0, then there exists a solution
// if a = 0 and b = 0 -> (c = 0 then infinite solutions, otherwise no solution)
// a * x0 * (c / gcd(a, b)) + b * y0 * (c / gcd(a, b)) = c
// x = x0 * (c / gcd(a, b)), y = y0 * (c / gcd(a, b))

int gcd(int a, int b, int &x, int &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    int x1, y1;
    int d = gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

bool find_any_solution(int a, int b, int c, int &x0, int &y0, int &g)
{
    g = gcd(abs(a), abs(b), x0, y0);
    if (c % g)
    {
        return false;
    }

    x0 *= c / g;
    y0 *= c / g;
    if (a < 0)
        x0 = -x0;
    if (b < 0)
        y0 = -y0;
    return true;
}

bool find_any_non_negative_solution(int a, int b, int c, int &x0, int &y0, int &g)
{

    if(!find_any_solution(a, b, c, x0, y0, g))
        return false;

    // find the solution with x >= 0 and y >= 0
    int k = 0;
    a /= g;
    b /= g;
    if(x0 < 0){
        k = (abs(x0) + b - 1) / b;
        x0 += k * b;
        y0 -= k * a;
    }
    if(y0 < 0){
        k = (abs(y0) + a - 1) / a;
        x0 -= k * b;
        y0 += k * a;
    }
    return (x0 >= 0 && y0 >= 0);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// getting all solutions
// by adding b / g to x0 and subtract a / g from y0 will not break the equality
// x = x0 + k * (b / g), y = y0 - k * (a / g)

// Finding the number of solutions and the solutions in a given interval
// suppose minx <= x <= maxx, miny <= y <= maxy
// use linear diophantine equation to find any solution
// we shift the solution to have x >= minx and denote it as lx1
// we shift the solution to have x <= maxx and denote it as rx1
// we shift the solution to have y >= miny and denote it as lx2
// we shift the solution to have y <= maxy and denote it as rx2
// the final solution is all solutions with x in intersection of [lx1, rx1] and [lx2, rx2]

void shift_solution(int &x, int &y, int a, int b, int cnt)
{
    x += cnt * b;
    y -= cnt * a;
}

int find_all_solutions(int a, int b, int c, int minx, int maxx, int miny, int maxy)
{
    int x, y, g;
    if (!find_any_solution(a, b, c, x, y, g))
        return 0;
    a /= g;
    b /= g;

    int sign_a = a > 0 ? +1 : -1;
    int sign_b = b > 0 ? +1 : -1;

    shift_solution(x, y, a, b, (minx - x) / b);
    if (x < minx)
        shift_solution(x, y, a, b, sign_b);
    if (x > maxx)
        return 0;
    int lx1 = x;

    shift_solution(x, y, a, b, (maxx - x) / b);
    if (x > maxx)
        shift_solution(x, y, a, b, -sign_b);
    int rx1 = x;

    shift_solution(x, y, a, b, -(miny - y) / a);
    if (y < miny)
        shift_solution(x, y, a, b, -sign_a);
    if (y > maxy)
        return 0;
    int lx2 = x;

    shift_solution(x, y, a, b, -(maxy - y) / a);
    if (y > maxy)
        shift_solution(x, y, a, b, sign_a);
    int rx2 = x;

    if (lx2 > rx2)
        swap(lx2, rx2);
    int lx = max(lx1, lx2);
    int rx = min(rx1, rx2);

    if (lx > rx)
        return 0;
    return (rx - lx) / abs(b) + 1;
}
// Finding the number of solutions and the solutions in a given interval
// consider all cases any number can be in range -oo to +oo 
int find_all_solutions_pre(int a, int b, int c, int minx, int maxx, int miny, int maxy){
    if(a == 0 && b == 0){
        if(c == 0) return (maxx - minx + 1) * (maxy - miny + 1);
        return 0;
    }
    if(a == 0){
        if(c % b == 0 && miny <= c / b && c / b <= maxy) return maxx - minx + 1;
        return 0;
    }
    if(b == 0){
        if(c % a == 0 && minx <= c / a && c / a <= maxx) return maxy - miny + 1;
        return 0;
    }
    return find_all_solutions(a, b, c, minx, maxx, miny, maxy);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// find the solution with minimum x + y
// Here, x  and y  also need to be given some restriction, otherwise, the answer may become negative infinity.
// We find any solution of the Diophantine equation, and then shift the solution to satisfy some conditions.
// We can shift the solution to have x + y = x0 + y0 + k * (b / g - a / g) = x0 + y0 + k * (b - a) / g.
// If a<b , we need to select the smallest possible value of k.
// If a>b , we need to select the largest possible value of k.
// If a=b , all solution will have the same sum   x + y.


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// solve ax + by + cz = d
// gcd(a, b)(ax / gcd(a, b) + by / gcd(a, b)) + cz = d // denote w = ax / gcd(a, b) + by / gcd(a, b)
// gcd(a, b)w + cz = d
// if gcd(a, b, c) divides d, then there exists a solution, otherwise no solution
// denote g = gcd(a, b, c)
// w = w0 + k * (c / g), z = z0 - k * (gcd(a, b) / g)
// (ax / gcd(a, b) + by / gcd(a, b)) = w0 + k * (c / g)
// denote wc = gcd(a, b)
// x = x0 * (w0) + p * (b / wc), y = y0 * (w0) - p * (a / wc), z = z0 - k * (gcd(a, b) / g)
//! this function is manually implemented i don't know if it's correct or not
bool find_any_solution3(int a, int b, int c, int d, int &x0, int &y0, int &z0, int &g)
{
    int wc = __gcd(a, b); // coeeficient of w
    g = __gcd(wc, c); // gcd of a, b, c
    if (d % g) // if gcd(a, b, c) doesn't divide d, then no solution
        return false;

    int w0;
    if(!find_any_solution(wc, c, d, w0, z0, g)) // find any solution of wc * w + c * z = d
        return false;
    
    gcd(a, b, x0, y0); // find any solution of a * x + b * y = gcd(a, b)
    int k = 0, p = 0; // you can change these values to get different solutions
    w0 = w0 + k * (c / g);
    x0 = x0 * w0 + p * (b / wc);
    y0 = y0 * w0 - p * (a / wc);
    z0 = z0 - k * (wc / g);
    return true;
}
void solve(int tc)
{
    int a, b, c, x1, x2, y1, y2;
    cin >> a >> b >> c >> x1 >> x2 >> y1 >> y2;
    c = -c;
    int ans = find_all_solutions_pre(a, b, c, x1, x2, y1, y2);
    cout << ans << nl;
}
signed main(void)
{
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    int tc = 1;
    cin >> tc;
    int i = 1;
    while (tc--)
    {
        cout<<"Case "<<i<<": ";
        solve(i++);
    }
    return 0;
}
