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
int dp[1<<N][N]; // dp[x][i] is the shortest 0->i path visiting set bits of x


i32 main () {
    int m, k;
    cin >> n >> m >> k;
    // satisfactions
    int s[n];
    for (int i = 0; i < n; i++) cin >> s[i];
    for (int i = 0; i < k; i++)  {
        int x, y, c;
        cin >> x >> y >> c;
        // bonus
        adj[x - 1][y - 1] = c;
    }

    // setting vlaues to -inf probs not needed
    // for (int mask = 0; mask < (1<<n); mask++) {
    //     for (int i = 0; i < n; i++) {
    //         dp[mask][i] = -INF;
    //         cout << dp[mask][i] newline;
    //     }
    // }

    // base cases for any start dish - singleton
    for (int i = 0; i < n; i++) {
        dp[1 << i][i] = s[i];
    }
 

  for (int mask = 1; mask < (1<<n); mask++) {
    // can only eat up to m
    if (popcount((uint) mask) >= m) continue;
    for (int cur = 0; cur < n; cur++) {
        // only care if previosu choosen food is in mask
        if (mask & (1<<cur)) { 
            // current score
            int cdp = dp[mask][cur]; 
            for (int nxt = 0; nxt < n; nxt++) {
                if (!(mask & (1<<nxt))) { 
                    // if not choosen choose food
                    dp[mask|(1<<nxt)][nxt] = max(dp[mask|(1<<nxt)][nxt], cdp + adj[cur][nxt] + s[nxt]);
                }
            }

        }
    }
  }

    int ans = 0;
    for (int set = 0; set < (1<<n); set++) {
        for (int i = 0; i < n; i++) {
            ans = max(ans, dp[set][i]);
        }
    }
    cout << ans newline;

    return 0;
}
