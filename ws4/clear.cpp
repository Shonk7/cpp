// Source: https://usaco.guide/general/io

#include <bits/stdc++.h>
using namespace std;

int main() {

    int n;
    cin >> n;
    string s;
    cin >> s;
   
    vector<vector<int>> dp(n, vector<int>(n, 0));
    
    // Triangle dp
    for (int i = 0; i < n; ++i) {
        dp[i][i] = 1;
    }

    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;

            // basic choice
            dp[i][j] = 1 + dp[i + 1][j];

            // maybe we can delete something inside thats better
            for (int k = i + 1; k <= j; ++k) {
                if (s[i] == s[k]) {
                    int inner_cost = (i + 1 <= k - 1) ? dp[i + 1][k - 1] : 0;
                    dp[i][j] = min(dp[i][j], inner_cost + dp[k][j]);
                }
            }
        }
    }

    cout << dp[0][n - 1] << "\n";

    return 0;
}
