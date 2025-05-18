struct Combi{
private:
    static constexpr int NN = 1e6 + 5, MOD = 1e9 + 7;
    ll FACT[NN], FACTINV[NN];
public:
    ll fact(ll Num){return Num >= 0 ? FACT[Num] : 0;}
    ll fact_inv(ll Num){return Num >= 0 ? FACTINV[Num] : 0;}

    // fast power (Binary Exponention)
    ll fastpower(ll base, ll po, ll md) {
        ll ANS = 1;
        while (po){
            if (po & 1)
                ANS = (ANS * base) % md;
            base *= base, base %= md;
            po >>= 1;
        }
        return ANS;
    }

    // Mod Invers Num^(-1) % md
    ll mod_inv(ll Num, ll md){
        return fastpower(Num, md - 2, md);
    }

    void PreCalc() {
        FACT[0] = 1, FACTINV[0] = 1;
            for (int i = 1; i <= 1e6; i++)
                FACT[i] = (i * FACT[i - 1]) % MOD, FACTINV[i] = mod_inv(FACT[i], MOD);
    }

    ll nPr(ll n, ll r){
        return fact(n) * fact_inv(n - r) % MOD;
    }
    ll nCr(ll n, ll r){
        return fact(n) * fact_inv(n - r) % MOD * fact_inv(r) % MOD;
    }
};
