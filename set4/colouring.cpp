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

const int N = 110;
const int INF = 1e18;
vector<vector<vector<int>>> dp(N, vector<vector<int>>(N, vector<int>(N, INF)));
int c[N]; 
int cost[N][N];

int n;
i32 main() {
    fastio;
    int m, o;
    // o is k fromt he question i will use k as a iterator
    cin >> n >> m >> o;
    // colour of trees
    for (int i = 1; i <= n; i++) cin >> c[i];
    for (int i = 1; i <= n; i++) {
        // cost of painting
        for (int j = 1; j <= m; j++) cin >> cost[i][j];
    } 

    // first tree will create 1 beauty
    // if already coloured
    if (c[1] != 0) {
        dp[1][c[1]][1] = 0;
    } else {
        // try colouring other ways
        for (int j = 1; j <= m; j++) {
            dp[1][j][1] = cost[1][j];
        }
    }

    for (int i = 2; i <= n; i++) {
        // go through all previous colour choices
        for (int prev = 1; prev <= m; prev++) {
            for (int j = 1; j <= m; j++) {
                // if coloured soemthing else do this check here cause we need to assign the correct dp value even if
                // its alrready coloured
                if (c[i] != 0 && c[i] != j) continue;
                int paintC = (c[i] == j) ? 0 : cost[i][j];
                // new beautey if colour changes
                int b = (prev != j);
                for (int k = 1; k <= o; k++) {
                    // if (k + b > o) continue;
                    dp[i][j][k + b] = min(
                        dp[i][j][k + b],
                        dp[i - 1][prev][k] + paintC
                    );
                }
            }
        }

    }

    int ans = INF;
    // go through dp for every possible ednign colour
    for (int j = 1; j <= m; j++) {
        ans = min(ans, dp[n][j][o]);
    }

    cout << ((ans == INF) ? -1 : ans) newline;
    return 0;
}