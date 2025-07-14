struct Divs {

    ll sum_n(ll n) {return n * (n + 1) / 2;}
    ll sum_n_2(ll n) {return n * (n + 1) * (2 * n + 1) / 6;}
    ll sum_n_3(ll n) {return n * n * (n + 1) * (n + 1) / 4;}


    /* Check whether a number is Prime or not in O(log(N))  */
    bool is_prime(ll Number){
        if(Number < 2 || (Number % 2 == 0 && Number != 2)) return false;
        for(ll i = 3; i * i <= Number; i += 2)
            if(Number % i == 0) return false;
        return true;
    }

    /* Get the prime factorization of a number  */
    vector< pair<ll, int> > pFactors(ll num) {
        int fq = 0;
        vector < pair <ll, int> > factors;
        if (num % 2 == 0){
            fq = 0;
            while (num % 2 == 0)    fq++, num /= 2;
            factors.emplace_back(2, fq);
        }

        for (ll i = 3; i * i <= num; i += 2)
            if (num % i == 0) {
                fq = 0;
                while (num % i == 0)    fq++, num /= i;
                factors.emplace_back(i, fq);
            }
        
        if (num > 2)    factors.emplace_back(num, 1);

        return factors;
    }

    /* Get all divisors of a number (factors of the number) */
    vector <ll> get_divs(ll num) {
        vector<ll> divs;
        for (ll i = 1; i * i < num; i++)
            if (num % i == 0)
                divs.emplace_back(i),
                divs.emplace_back(num / i);
        ll sq = sqrtl(num);
        if (sq * sq == num)
            divs.emplace_back(sq);
        
        return divs;
    }

    /* how many numbers in the range [l, r] that are divisible by d */
    ll numDivs_inRangeLR(ll l, ll r, ll d) {
        if (l > r)  swap(l, r);
        return r / d - (l - 1) / d;
    }

    ll get_sum_n(ll num) {
        return num * (num + 1) / 2;
    }

    /* sum of the numbers in the range [l, r] that are divisible by d */
    ll sumDivs_inRangeLR(ll l, ll r, ll d) {
        if (l > r)  swap(l, r);
        ll left = get_sum_n((l - 1) / d);
        ll right = get_sum_n(r / d);
        return (right - left) * d;
    }

    /* sum of divisors of a number */
    ll sumDivs(ll num) {
        ll sum = 0;
        for (ll i = 1; i * i < num; i++)
            if (num % i == 0)
                sum += i + num / i;
        ll sq = sqrtl(num);
            if (sq * sq == num)
                sum += sq;
        return sum;
    }

    /* number of divisors of a number */
    ll numDivs(ll num) {
        ll ret = 0;
        for (ll i = 1; i * i < num; i++)
            if (num % i == 0)
                ret += 2;
        ll sq = sqrtl(num);
            if (sq * sq == num)
                ret++;
        return ret;
    }

    /* Number of prefect sqaures between l and r */
    ll num_of_pref_squares(ll l, ll r){
        if (l > r)  swap(l, r);
        return ((ll)(sqrtl(r)) - ceil(sqrtl(l)) + 1);
    }


    // phi(N) -> count how many numbers x less than n and gcd(x, n) == 1 (Co-prime)
    ll phi(ll num) {
        ll ph = 1, curFq = 0;
    
        if (num % 2 == 0){
            curFq = 0;
            while (num % 2 == 0)    curFq++, num /= 2;
            ph *= powl(2, curFq) - powl(2, curFq - 1);
        }
    
        for (int i = 3; i <= sqrt(num); i += 2)
            if (num % i == 0){
                curFq = 0;
                while (num % i == 0)    curFq++, num /= i;
                ph *= powl(i, curFq) - powl(i, curFq - 1);
            }
        
        if (num > 2)
            ph *= num - 1;
        
        return ph;
    }

    // n! % p^x  -> We wanna find the larest x   
    ll largest_power_divFactoN(ll n, ll p) {
        return n ? n / p + largest_power_divFactoN(n / p, p) : 0;
    }

};
