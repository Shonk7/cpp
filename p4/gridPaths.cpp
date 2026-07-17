#include <bits/stdc++.h>
using namespace std;

#define i32 int32_t
#define int long long
#define uint unsigned long long

#define fastio        \
    cin.tie(nullptr); \
    cin.sync_with_stdio(false);

#define all(c) c.begin(), c.end()
#define space << " "
#define newline << "\n"
#include <bit>

using namespace std;
int n;
const int N = 1010;
const int INF = 1e9;
const int MOD = 1e9 + 7;
int dp[N][N];

i32 main () {
    int m, k;
    cin >> n ;
    char grid[n][n];
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }
    dp[0][0] = 1;
    if (grid[0][0] == '*') dp[0][0] = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i > 0 && grid[i - 1][j] != '*') dp[i][j] = (dp[i][j]  + dp[i - 1][j]) % MOD ;
            if (j > 0 && grid[i][j - 1] != '*') dp[i][j] = (dp[i][j]  + dp[i][j - 1]) % MOD ;
      }
    }
    if (grid[n - 1][n - 1] == '*') dp[n - 1][n - 1] = 0;
    cout << dp[n - 1][n - 1] newline;

    return 0;
}
