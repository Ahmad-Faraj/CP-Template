#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
//-------------------------------------------------------------------------------------------------------
using namespace std;
using namespace __gnu_pbds;
//-------------------------------------------------------------------------------------------------------
template <typename K, typename V, typename Comp = std::less<K>> using __ordered_map = tree<K, V, Comp, rb_tree_tag, tree_order_statistics_node_update>;
template <typename K, typename Comp = std::less<K>> using ordered_set = __ordered_map<K, null_type, Comp>;
template <typename K, typename V, typename Comp = std::less_equal<K>> using __ordered_multimap = tree<K, V, Comp, rb_tree_tag, tree_order_statistics_node_update>;
template <typename K, typename Comp = std::less_equal<K>> using ordered_multiset = __ordered_multimap<K, null_type, Comp>;
//-------------------------------------------------------------------------------------------------------
typedef vector <int> vi;
typedef vector <long long> vl;
typedef pair <int, int> pi;
typedef pair <long long, long long> pl;
typedef vector < pair <int, int> > vpi;
typedef vector < pair <long long, long long> > vpl;
typedef complex <double> point;
//-------------------------------------------------------------------------------------------------------
#define aat(Idx) find_by_order(Idx) // returns iterator
#define idxof(Element) order_of_key(Element) // gets me idx of what equals (Element) or the what's after (Elelment)
#define cout_2d(vec, n, m) for(int iii = 0; iii < n; iii++, cout << "\n") for(int jjj = 0; jjj < m && cout << vec[iii][jjj] << " "; jjj++);
#define cin_2d(vec, n, m) for(int iii = 0; iii < n; iii++) for(int jjj = 0; jjj < m && cin >> vec[iii][jjj]; jjj++);
string get_binary(long long num){if(!num)return"";return get_binary(num/2)+(num%2?"1":"0");}
#define cout_map(mp) for(auto& [fff, sss] : mp) cout << fff << "  " << sss << "\n";
#define cout(vec) for(auto& iii : vec) cout << iii << " "; cout << "\n";
#define cin(vec) for(auto& iii : vec) cin >> iii
#define to_decimal(bin) stoll(bin, nullptr, 2)
#define unq(VEC) sort(all(VEC)); VEC.resize(unique(all(VEC)) - VEC.begin());
#define ll long long
#define ull unsigned long long
#define Num_of_Digits(n) ((int)log10(n) + 1)
#define myCeil(N, M) (N / M + bool(N % M))
#define dg(x) cout << #x << " : " << (x) << "\n"
#define sz(x) (int)(x.size())
#define all(vec) vec.begin(), vec.end()
#define rall(vec) vec.rbegin(), vec.rend()
#define ff first
#define ss second
#define pp push_back
#define er cout << "Error Here!!\n"
#define OO (int)(1 << 30)
#define OOL (1LL << 60)
#define MOD (int)(1e9 + 7)
#define el "\n"
#define sp " "
#define EPS 1e-9
#define PI acos(-1.0)
#define xx real()
#define yy imag()
#define angle(POINT) atan2(POINT.yy, POINT.xx)  // tan-1(y / x)
#define vec(POINT_A, POINT_B) complex<double>((POINT_B) - (POINT_A))
#define dotp(POINT_A, POINT_B) ((conj(POINT_A) * (POINT_B)).real())  // a*b cos(T), if zero -> perpendicular
#define crossp(POINT_A, POINT_B) ((conj(POINT_A) * (POINT_B)).imag())  // a*b sin(T), if zero -> parllel
#define length(POINT) (hypot((POINT).imag(), (POINT).real()))  // hypot is better than sqrt because sqrt may cause overflow, on the other hand hypot is slower "TLE"
#define normalize(POINT) (POINT) / length(POINT)
#define lengthSqr(POINT) dotp(POINT, POINT);  // use dotp(x, x) instead of norm(x)
#define areSame(POINT1, POINT2) (dotp(vec(POINT1, POINT2), vec(POINT1, POINT2)) < EPS)
#define rotateO(POINT, ANGLE) ((POINT) * exp(point(0, ANGLE)))   // exp(L) => e^L
// p = r * e^(iθ), ∴ when rotating we just change the angle θ, ∴ so the angle become θ + α, ∴ p become p * e^(α)
#define rotateA(POINT, ANGLE, ABOUT)  (rotateO(vec(ABOUT, POINT), ANGLE) + ABOUT)  // we rotate around the "about" considering it as the origin, then we shift after rotation
#define reflectO(VEC, MMM)  (conj((VEC) / (MMM)) * (MMM))
point reflect(point p, point p0, point p1) {point z = p-p0, w = p1-p0;return conj(z/w)*w + p0;} /* Refelect point p around p0p1 */
double fixAngle(double A) {return A > 1 ? 1 : (A < -1 ? -1 : A);}
int dcmp(double a, double b) {return fabs(a - b) <= EPS ? 0 : a < b ? -1 : 1;}
bool isCollinear(point A,point B,point C){return fabs(crossp(B-A, C-A))<EPS;}
bool isPaboveAB(point A, point B, point p){return fabs(crossp(B-A, p-A))>EPS;}
bool isPointOnRayAB(point A,point B,point p){return!isCollinear(A,B,p)?0:(dcmp(dotp(B-A, p-A),0)==1);}
bool isPointOnSegAB(point A,point B,point p){return isPointOnRayAB(A,B,p)&&isPointOnRayAB(B,A,p);}
double distP_ToLineAB(point A,point B,point p){return fabs(crossp(B-A,p-A)/length(A-B));}
//-------------------------------------------------------------------------------------------------------
vector < pair <int, int> > dxy4 = {{1, 0}, {0, 1}, {0, -1}, {-1, 0}};
vector < pair <int, int> > dxy8 = {{1, -1}, {0, -1}, {-1, -1}, {1, 0}, {-1, 0}, {1, 1}, {0, 1}, {-1, 1}};
vector < pair <int, int> > dxyk = {{-1, -2}, {-2, -1}, {1, -2}, {2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, 1}};
//----------------------------------------------CODE___HERE----------------------------------------------

ld SectorArea(ld r, ld angle) { // مساحة القطاع الدائري
    0.5 * r * r * angle;
}

ld PiceArea(ld r, ld angle) {   // مساحة القطعة الدائرية
    ld TriangleArea = 0.5 * r * r * sin(angle);
    return SectorArea(r, angle) - TriangleArea;
}

ld areaOfRegPolygon(ld n, ld sideLen) {
    return n * sideLen * sideLen * 0.25 * (1 / tan(PI / n));
}

double Rad(double deg)
{
    return (deg * PI) / 180.0;
}
double Deg(double rad)
{
    return (rad * 180) / PI;
}

double fixAngle(double An)
{
    return An > 1 ? 1 : (An < -1 ? -1 : An);
}

double getSide_a_bAB(double b, double A, double B)
{
    return (b * sin(A)) / sin(B);
}

double getAngle_A_abB(double a, double b, double B)
{
    return asin(fixAngle((a * sin(B)) / b));
}

double getAngle_A_abc(double a, double b, double c)
{
    return acos(fixAngle((a * a - b * b - c * c) / (-2 * b * c)));
}



//-------------------------------------------------------------------------------------------------------
void SOLVE() {
    
}
//-------------------------------------------------------------------------------------------------------
void Sanad();
signed main() {
    Sanad();
    // cout << fixed << setprecision(10);
    int testcases = 1;
    // cin >> testcases;
    while (testcases--)
        SOLVE();
    // cerr << "Time Taken: " << (float)clock() / CLOCKS_PER_SEC << " Secs" << "\n";
    return 0;
}
//-------------------------------------------------------------------------------------------------------
void Sanad() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    // freopen("##.in", "r", stdin);
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    #endif
}
