#include <bits/stdc++.h>

#define ll long long
#define int long long
#define ld long double

#define nl "\n"
#define sp " "
#define EPS (1e-12)

#define all(p) p.begin(), p.end()
#define fixed(n) fixed << setprecision(n)

using namespace std;

void Fast_IO()
{
// ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif
}

template<typename T = int>
struct XOR_Basis{
    int N, length, base_rank;
    vector<T> Matrix;

    XOR_Basis(int __n = -1){
        N = __n, length = 0, base_rank = 0;
        if(N == -1) N = sizeof(T) * 8;
        Matrix.assign(N, 0);
    }
    
    int get_msb(T x){ // most significant bit
        return 63 - __builtin_clzll(x);
    }

    int get_rank_of_matrix(){
        return base_rank;
        int ans = 0;
        for(int i = 0; i < N; i++){ ans += (Matrix[i] != 0); }
        return ans;
    }

    void add_element(T x){
        T min_to_add = x;
        length++;
        int msb = -1;
        for(int i = N - 1; i >= 0; i--){
            min_to_add = min(min_to_add, min_to_add ^ Matrix[i]);
            if(msb == -1 && ((min_to_add >> i) & 1)) msb = i;
        }
        if(min_to_add){
            Matrix[msb] = min_to_add; base_rank++;
        }
    }
    
    T get_max_xor(){
        T ans = 0;
        for(int i = N - 1; i >= 0; i--){
            ans = max(ans, ans ^ Matrix[i]);
        }
        return ans;
    }

    bool can_make(T x, bool exclude_the_empty_set = 0){
        if(exclude_the_empty_set && x == 0) { return length > base_rank; }
        T min_to_add = x;
        for(int i = N - 1; i >= 0; i--){
            min_to_add = min(min_to_add, min_to_add ^ Matrix[i]);
        }
        return min_to_add == 0;
    }

    int count_equal_to_x(T x, int len = length){
        return (can_make(x) ? (1LL << (len - base_rank)) : 0);
    }

    // from 0 to (2^base_rank - 1):
    T get_kth_smallest_xor(int k){ 
        if((k >= (1 << base_rank)) || k < 0) return -1;
        T ans = 0;
        for(int i = 0, bit = -1; i < N; i++){
            bit += (Matrix[i] != 0);
            if((k >> bit) & 1){
                ans ^= Matrix[bit];
            }
        }
        return ans;
    }

    // from 0 to (2^base_rank - 1)
    T get_kth_greater_xor(int k){
        if((k >= (1 << base_rank)) || k < 0) return -1;
        return get_kth_smallest_xor(get_span_of_Matrix() - k - 1);
    }

    long long get_span_of_Matrix(){
        return (1LL << base_rank);
    }

    long long count_numbers(){
        return get_span_of_Matrix();
    }
};


void Solve(){
    
    
}

// static bool start = []() {preCalc(); return true; }();
signed main()
{
	ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);
	Fast_IO();

	int tc = 1;
	cin >> tc;


	while (tc--)
	{
		Solve();
	}

	return 0;
}
