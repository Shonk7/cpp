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
const int N = 20;
const int INF = 1e9;
int adj[N][N]; 
// rows, states
int dp[N][1 << N];
//vector<int> dp(1 << N, INF);
// int cost[1 << N];
int grid[N][N];
int rowScore[N][1 << N];
i32 main () {
    cin >> n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }

    // precompute row score for each mask
    for (int r = 0; r < n; r++ ) {
        for (int mask = 0; mask < (1<<n); mask++) {
            int sum = 0;
            // go through each pebble
            for (int p = 0; p < n; p++) {
                if (mask & (1 << p)) {
                    sum += grid[r][p];
                }
            }
            rowScore[r][mask] = sum;
        }
    }
    // base case
    for (int c = 0; c < (1<<n); c++) {
        dp[0][c] = max(dp[0][c], rowScore[0][c]);
    }
    // Idea is go through every mask pair for each row 
    // and if they are comaptible work out score
    for (int r = 1; r < n; r++ ) {
        for (int c = 0; c < (1<<n); c++) {
            if (c & (c << 1)) continue;
            for (int p = 0; p < (1<<n); p++) {
                if (p & (p << 1)) continue;
                // check compatible 
                if (c & p || c & (p >> 1) || c & (p << 1)) continue;

                dp[r][c] = max(dp[r][c], dp[r - 1][p] + rowScore[r][c]);
            }
        }
    }
    
    int ans = 0;
    for (int mask = 0; mask < (1<<n); mask++) {
        ans = max(ans, dp[n - 1][mask]);
        
    }
    cout << ans newline;

    return 0;


}
