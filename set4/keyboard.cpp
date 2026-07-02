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
// int dp[1 << N];
vector<int> dp(1 << N, INF);
int cost[1 << N];

i32 main () {
    int m;
    cin >> n >> m;
    string s;
    cin >> s;
    // countign adjancencies
    for (int i = 1; i < n; i++)  {
        adj[s[i - 1] - 'a'][s[i] - 'a']++;
        adj[s[i] - 'a'][s[i - 1] - 'a']++;
    }
    // cost of current state i.e. the sum of pairs with one in the mask
    // and an opposing one outside the mask
    // after each choice i.e. adding of a new letter.
    // those pairs not slected will increment the total cost by 1. (or adj[i][j] if there are copies)
    // doing this instead of dijkstras to work out the distance
    
    for (int mask = 0; mask < (1<<m); mask++) {
        for (int i = 0; i < m; i++) {
            // if not selected
            if (!(mask & (1 << i))) continue;
            for (int j = 0; j < m; j++) {
                // if selected
                if (mask & (1 << j)) continue;
                // now we have letters on seperate boundaries
                cost[mask] += adj[i][j];
            }
        }
    }
    
    dp[0] = 0;
    for (int mask = 0; mask < (1<<m); mask++) {
        for (int nxt = 0; nxt < m; nxt++) {
            // make sure to check not selected
            if (!(mask & (1<<nxt))) { 
                dp[mask|(1<<nxt)] = min(dp[mask|(1<<nxt)], dp[mask] + cost[mask]);
            }
        }
    }

    int ans = dp[(1 << m) - 1];
    cout << ans newline;

    return 0;


}
