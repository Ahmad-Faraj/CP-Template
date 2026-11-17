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
    int N, length, base_rank, r;
    vector<T> Matrix;
    vector<int> bit_last_inx;

    XOR_Basis(int __n = -1){
        N = __n, length = 0, base_rank = 0, r = -1;
        if(N == -1) N = sizeof(T) * 8;
        Matrix.assign(N, 0);
        bit_last_inx.assign(N, -1);
    }
    
    int get_msb(T x){ // most significant bit
        return 63 - __builtin_clzll(x);
    }

    int get_rank_from_l(int l = 0){
        int ans = 0;
        for(int i = N - 1; i >= 0; i--) 
            if(Matrix[i] != 0 && bit_last_inx[i] >= l) ans++;
        return ans;
    }

    void add_element(T x, int inx = 0){
        length++, r = inx;
        for(int i = N - 1; i >= 0; i--){
            if(!((x >> i) & 1)) continue;
            if(bit_last_inx[i] < inx){
                if(Matrix[i] == 0) base_rank++;
                swap(Matrix[i], x);
                swap(inx, bit_last_inx);
            }
            x ^= Matrix[i];
        }
    }

    T get_max_xor_from_l(int l = 0){
        T ans = 0;
        for(int i = N - 1; i >= 0; i--){
            if(bit_last_inx[i] >= l) ans = max(ans, ans ^ Matrix[i]);
        } 
        return ans;
    }

    bool can_make_from_l(T x, int l = 0, bool exclude_the_empty_set = 0){
        if(exclude_the_empty_set && x == 0) { return length > get_rank_from_l(l); }
        T min_to_add = x;
        for(int i = N - 1; i >= 0; i--){
            min_to_add = min(min_to_add, min_to_add ^ Matrix[i]);
        }
        return min_to_add == 0;
    }

    int count_equal_to_x_from_l(T x, int l = 0){
        return (can_make_from_l(x) ? (1LL << ((r - l + 1) - get_rank_from_l(l))) : 0);
    }

    // from 0 to (2^base_rank - 1):
    T get_kth_smallest_xor_from_l(int k, int l = 0){ 
        if((k >= (1 << get_rank_from_l(l))) || k < 0) return -1;
        T ans = 0;
        for(int i = 0, bit = -1; i < N; i++){
            bit += (Matrix[i] != 0 && bit_last_inx[i] >= l);
            if((k >> bit) & 1){
                ans ^= Matrix[bit];
            }
        }
        return ans;
    }

    // from 0 to (2^base_rank - 1)
    T get_kth_greater_xor_from_l(int k, int l = 0){
        return get_kth_smallest_xor_from_l(get_span_of_Matrix_from_l() - k - 1, l);
    }

    long long get_span_of_Matrix_from_l(int l = 0){
        return (1LL << get_rank_from_l(l));
    }

    long long count_numbers_from_l(int l = 0){
        return get_span_of_Matrix(l);
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
