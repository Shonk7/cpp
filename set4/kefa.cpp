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

using namespace std;
int n;
i32 main() {
    cin >> n;
    vector<vector<int>> h(n, vector<int>(2, 0));
    vector<vector<int>> dp(n + 1, vector<int>(2, 0));
    for (int i = 0; i < n; i++) cin >> h[i][0];
    for (int i = 0; i < n; i++) cin >> h[i][1];
    // dp[0][0] = 0;
    // dp[1][0] = 0;
    dp[1][0] = h[0][0];
    dp[1][1] = h[0][1];
    for (int i = 2; i <= n; i++) {
        for (int j = 0; j < 2; j++) {
            dp[i][j] = max({
                dp[i-1][(j+1) % 2] + h[i - 1][j],
                dp[i-2][(j+1) % 2] + h[i - 1][j],
                dp[i-2][j] + h[i - 1][j],
                dp[i-1][j]
            });
        }
    }

    cout << max(dp[n][0], dp[n][1]) newline;
    return 0;
}