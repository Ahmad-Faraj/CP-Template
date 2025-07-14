struct Sieve {

    /* Sieve Is This number prime or not */
    vector <bool> IsPrime;
    vector <int> primeNumbers;
    void preCalcPrimes(int NN = 1e6 + 5) {
        IsPrime.resize(NN);
        for (int i = 2; i < NN; i++) IsPrime[i] = 1;
        for (int i = 4; i < NN; i += 2)    IsPrime[i] = 0;
        for (int i = 3; i * i < NN; i += 2)    if (IsPrime[i])
                for (int j = i * i; j < NN; j += i + i)
                    IsPrime[j] = 0;
        
        for (int i = 2; i < NN; i++)if (IsPrime[i]) primeNumbers.push_back(i);
    }

    /* Sieve Get number of factors */
    vector<int> NumFactors;
    void preCalcNumFactors(int NN = 1e6 + 5) {
        NumFactors[0] = 0, NumFactors[1] = 1;
        for (int i = 2; i < NN; i++)   NumFactors[i] = 2;
        for (ll i = 2; i < NN; i++)
            for (ll j = i + i; j < NN; j += i)
                NumFactors[j]++;
    }

    /* Sieve Factors */
    vector<int> theFactors[2000015];
    void preCalcFactors(int NN = 2e6 + 5) {
        theFactors[1].emplace_back(1);
        for (int i = 2; i < NN; i++){
            theFactors[i].emplace_back(1);
            theFactors[i].emplace_back(i);
        }
    
        for (ll i = 2; i < NN; i++)
            for (ll j = i + i; j < NN; j += i)
                theFactors[j].push_back(i);
    }

    /* Sieve Get the SPF (Smallest Prime Factor) */
    vector<int> spf;
    void preCalcSPF(int NN = 1e6 + 5) {
        for (int i = 1; i < NN; i++)    spf[i] = i;
        for (int i = 2; i < NN; i+=2)   spf[i] = 2;
        for (int i = 3; i * i < NN; i+=2)   if (spf[i] == i)
                for (ll j = i * i; j < NN; j+=i)
                    spf[j] = min(spf[j], i);
    }

    // get prime factors by SPF
    vector<int> PrimeFactorsSPF(int Num) {
        vector<int> theFactors;
        while (Num != 1)
            theFactors.push_back(spf[Num]), Num /= spf[Num];
        return theFactors;
    }
};
