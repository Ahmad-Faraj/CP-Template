#include <bits/stdc++.h>

#define ll long long
#define ld long double

#define nl "\n"
#define sp " "

#define fixed(n) fixed << setprecision(n)

#include <ext/pb_ds/assoc_container.hpp> 
#include <ext/pb_ds/tree_policy.hpp> 
using namespace __gnu_pbds; 
// #define ordered_set tree<int, null_type,less<int>, rb_tree_tag,tree_order_statistics_node_update>  
#define ordered_set tree<long long, null_type,greater_equal<long long>, rb_tree_tag,tree_order_statistics_node_update> 

using namespace std;

void Fast_IO(){
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    #endif
}

// ----------------------------- {code} ----------------------------
template <typename T = double>
struct Point{
    T x,y;
    Point(){}
    Point(T x_, T y_){x = x_, y = y_;}

    Point operator - (Point &other){
        Point res(x - other.x, y - other.y);
        return res;
    } 
    void operator -= (Point &other){
        x -= other.x;
        y -= other.y;
    }

    T getLength(){
        return sqrtl(powl(fabs(x), 2) + powl(fabs(y), 2));
    }

    T getLengthTo(Point &other){
        return sqrt(powl(fabs(x - other.x), 2) + powl(fabs(y - other.y), 2));
    } 

    T getLengthBetween(const Point &A,const Point &B){
        return sqrt(powl(fabs(A.x - B.x), 2) + powl(fabs(A.y - B.y), 2));
    }

};

template <typename T = double>
struct MyGeoForGeometry{

    MyGeoForGeometry(){}

    T getAreaOfTriangleWithLengths(T a, T b, T c){
        T s = (a + b + c) * 0.5;
        return sqrtl((s - a) * (s - b) * (s - c) * s);
    }

    T getAreaOfTriangleWithPoint(Point<T> A, Point<T> B, Point<T> C){
        T a = B.getLengthTo(C), b = A.getLengthTo(C), c = A.getLengthTo(B);
        return getAreaOfTriangleWithLengths(a, b, c);
    }

    T getAreaOfTriangleWithMedians(T m1, T m2, T m3){
        if(m1 <= 0 || m2 <= 0 || m3 <= 0) return -1;
        T area = (4.0/3.0) * getAreaOfTriangleWithLengths(m1, m2, m3);
        if(area <= 0) return -1;
        return area;
    }

    T dotProd(Point<T> V, Point<T> W){
        return (V.x*W.x + V.y*W.y);
    }

    T getAngleBetween2Vectors(Point<T> V, Point<T> W){
        return acosl(clamp( dotProd(V, W) / fabsl(V.getLength()) / fabsl(W.getLength()) , -1.0, 1.0 ) );
    }

    T crossProd(Point<T> V, Point<T> W){
        return (V.x*W.y - V.y*W.x);
    }

    T orient(Point<T> A, Point<T> B, Point<T> C){
        return crossProd(B - A, C - A);
    }

    bool isBetweenAngle(Point<T> A, Point<T> B, Point<T> C, Point<T> P){
        if(orient(A, B, C) == 0) return 0;
        return (orient(A, B, P) >= 0 && orient(A, C, P) <= 0);
    }

};


void Solve(){

}


int main(){
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    Fast_IO();

    int tc = 1;
    // cin>>tc;
    while (tc--)
    {   
        Solve();
    }
    
    return 0;
}


