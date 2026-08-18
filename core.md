## Setup Commands

### Fast File Generation
Run this in the terminal to immediately generate files A to N by copying your template:
```bash
cat main.cpp > {a..n}.cpp
// Alternative way to duplicate main.cpp
for file in {a..n}.cpp; do cp main.cpp "$file"; done
```

### VS Code Settings (`settings.json`)
Use these settings with the **Code Runner** extension to auto-save and redirect inputs/outputs efficiently.

**VS Code Settings**
- Run In Terminal: ✔️
- Save All Files Before Run: ✔️

**Executor Map Configuration:**
Add the following configuration in your VS Code settings to execute C++ code and redirect input/output:
```json
{
    "code-runner.executorMap": {
        "cpp": "g++ $fileName -o $fileNameWithoutExt && $dir/$fileNameWithoutExt < input.txt > output.txt"
    }
}
```
## Main Templete

```cpp
// وَأَن لَّيْسَ لِلْإِنسَانِ إِلَّا مَا سَعَىٰ (39) وَأَنَّ سَعْيَهُ سَوْفَ يُرَىٰ (40) ثُمَّ يُجْزَاهُ الْجَزَاءَ الْأَوْفَىٰ (41)
#include <bits/stdc++.h>
using namespace std;
    
    vector < pair < int, int > > dir_4 = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
    vector < pair < int, int > > dir_8 = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    vector < pair < int, int > > knight_mov = {{2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};

    long double PI = acosl(-1.0L) ;
    const int MOD = 1e9 + 7 ;
    const int oo = 0x3f3f3f3f ;
    const long long int OO = 1ll << 62 ;

#define ll long long
#define int long long
#define ld long double
#define popcnt(x) __builtin_popcountll( x )
#define all(vec) vec.begin(), vec.end()
#define sz(x) (int)x.size()
#define sub_mod(a, b) ((((a) % MOD) - ((b) % MOD) + MOD) % MOD)
#define add_mod(a, b) ((((a) % MOD) + ((b) % MOD)) % MOD)
#define mult_mod(a, b) ((((a) % MOD) * ((b) % MOD)) % MOD)

void solve ()
{

}

signed main() {
    
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t ;
//   cin >> t ;
    while ( t -- )
    {
        solve () ;
        cout << "\n" ;
    }

    return 0;
}
```
