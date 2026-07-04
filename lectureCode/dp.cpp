/*
 *
 * code for dp
 *
 */

/*
 * sum representation
 */

f[0] = 1, f[1] = 1, f[2] = 1, f[3] = 2;
for (int i = 4; i <= n; i++) {
  f[i] = f[i-1] + f[i-3] + f[i-4];
}

/*
 * sum representation memory optimised
 */

f[0] = 1, f[1] = 1, f[2] = 1, f[3] = 2;
for (int i = 4; i <= n; i++) {
  f[i%4] = f[(i-1)%4] + f[(i-3)%4] + f[i%4];
}


/*
 * implementing top down
 */

int f(int n) {
  // base cases
  if (n == 0 || n == 1) return 1;
  // return the answer from the cache if we already have one
  if (cache[n]) return cache[n];
  // calculate the answer and store it in the cache
  return cache[n] = f(n-1) + f(n-2);
}


/*
 * implementing bottom up
 */

f[0] = 1, f[1] = 1;
for (int i = 2; i <= n; i++) { f[i] = f[i-1] + f[i-2]; }


/*
 * knapsack top down
 */

const int INF = 1000*1000*1000+7;
const int UNKNOWN = -1;

const int N = 5050, MAXS = 5050;
int cache[N][MAXS]; // initialise to -1 because 0 is a valid answer

int f(int i, int j) {
  // base cases
  if (j < 0) return -INF;
  if (i == 0 || j == 0) { return 0; }
  // check cache
  if (cache[i][j] != UNKNOWN) { return cache[i][j]; }
  // calculate answer
  return cache[i][j] = max(f(i-1, j-s[i]) + v[i], f(i-1, j));
}

int solve (int n, int S) {
  for (int i = 0; i <= n; i++) { fill (cache[i], cache[i]+S+1, UNKNOWN); }
  return f(n, S);
}

/*
 * knapsack bottom up
 */

const int N = 5050, MAXS = 5050;
int dp[N][MAXS]; // zero-initialised; correct for i = 0 and j = 0

for (int i = 1; i <= n; i++) {
  // everything with smaller i will be solved by the time we get here
  for (int j = 1; j <= S; j++) {
    int m = dp[i-1][j]; // skip book i
    // take book i; bounds check to skip when array index is negative
    if (j - s[i] >= 0) { m = max(m, dp[i-1][j-s[i]] + v[i]); }
    dp[i][j] = m;
  }
}


/*
 * bitsets
 */

// for all sets
for (int set = 0; set < (1<<n); set++) {
  // for all subsets of that set
  for (int subset = set; subset; subset = (subset-1) & set) {
    // do something with the subset
  }
}


/*
 * roof tiling
 */

for (int set = 0; set < (1<<m); set++) {
  dp[m][set] = (popcount(set) >= k); // base case
}

for (int i = m+1; i <= n; i++) {
  fill(dp[i], dp[i] + (1<<m), 0);
  for (int set = 0; set < (1<<m); set++) if (popcount(set) >= k) {
    dp[i][set] = dp[i-1][set>>1]
               + dp[i-1][(set>>1)|(1<<(m-1))];
  }
}

long long ans = 0;
for (int set = 0; set < (1<<m); set++) { ans += dp[n][set]; }

/*
 * tsp
 */

#include <algorithm>
using namespace std;

const int N = 20;
const int INF = 1e9;
int n, adj[N][N]; // assume this is given.
int dp[1<<N][N]; // dp[x][i] is the shortest 0->i path visiting set bits of x

int tsp (void) {
  for (int mask = 0; mask < (1<<n); mask++)
    for (int city = 0; city < n; city++) { dp[mask][city] = INF; }
  dp[1][0] = 0; // 1 represents seen set {0}

  int ans = INF;
  for (int mask = 1; mask < (1<<n); mask++) // for every subset of cities seen so far
    for (int cur = 0; cur < n; cur++) {
      if (mask & (1<<cur)) { // cur must be one of the cities seen so far
        int cdp = dp[mask][cur]; // distance travelled so far
        if (mask == (1<<n) - 1)  { ans = min(ans, cdp); } // seen all cities
        // note we don't have to add adj[cur][0] to return to vertex 0
        for (int nxt = 0; nxt < n; nxt++)
          if (!(mask & (1<<nxt))) { // try going to a new city
            // new seen set is mask union {nxt}, and we will be at nxt
            // distance incurred to get to this state is now no worse than
            // cdp (current distance incurred) + edge from cur to nxt
            dp[mask|(1<<nxt)][nxt] = min(dp[mask|(1<<nxt)][nxt], cdp + adj[cur][nxt]);
          }
      }
    }
  return ans;
}


/*
 * fireworks top down
 */

int f(int i) {
  if (i == 0) return 0;
  if (cache[i]) return cache[i];
  int m = s[i];
  for (int j = 1; j < i; j++) {
    if (x[j] >= l[i] && x[j] <= r[i]) {
      m = max(m, f(j) + s[i]);
    }
  }
  return cache[i] = m;
}


/*
 * fireworks bottom up
 */

ll res = 0;

for (int i = 1; i <= n; i++) {
  dp[i] = s[i];
  for (int j = 1; j < i; j++) {
    // try the jth as the penultimate firework
    if (x[j] >= l[i] && x[j] <= r[i]) {
      dp[i] = max(dp[i], dp[j] + s[i]);
    }
  }
  // update answer
  res = max(res, dp[i]);
}


/*
 * fireworks bottom up range tree
 */

int query(int a, int b); // max query range tree of size 500,000
int update(int a, int v); // update index a to value v

ll res = 0;
for (int i = 1; i <= n; i++) {
  // calculate best score ending in i-th firework using the range tree
  dp[i] = query(l[i], r[i] + 1) + s[i];
  // add i-th firework to RMQ
  update(x[i], dp[i]);
  // update final answer if necessary
  res = max(res, dp[i]);
}


/*
 * segment cover 1
 */

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

typedef long long ll;
const int N = 100100;
const ll INF = (1LL << 61);
int n, m;

vector<pair<int, ll>> segments[N]; // (start, cost)
ll dp[N];
int tree[1<<18]; // range min tree with point update
void update(int p, ll v, int i = 1, int cL = 0, int cR = n);
ll query(int qL, int qR, int i = 1, int cL = 0, int cR = n); // [qL, qR)

int main() {
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int s, e, c;
        cin >> s >> e >> c;
        segments[e].emplace_back(s, c); // preprocess: collate by end point
    }
    
    for (int i = 0; i <= n; i++) { update(i, INF); }

    // to be continued


/*
 * segment cover 2
 */

    for (int i = 0; i <= n; i++) {
        dp[i] = INF;
        for (auto seg : segments[i]) {
            ll prevcost = seg.first == 0 ? 0 : query(seg.first-1, i);
            dp[i] = min(dp[i], prevcost + seg.second);
        }
        update(i, dp[i]);
    }
    cout << dp[n] << '\n';
}


/*
 * building 1
 */

#include <algorithm>
#include <iostream>
using namespace std;

typedef long long ll;

const int N = 1010;
ll a[N][N]; // (i,j)
ll dp[N][N][2]; // (j,h,b)

int main (void) {
    int H, W;
    cin >> H >> W;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cin >> a[i][j];
        }
    }


/*
 * building 2
 */

    ll M, S;
    for (int j = 1; j <= W; j++) {
        // b = 0 case 
        M = 0; S = 0;
        dp[j][0][0] = M + S;
        for (int h = 1; h <= H; h++) {
            M = max(M, dp[j-1][h][0]);
            S += a[H-h][j-1];
            dp[j][h][0] = M + S;
        }
        // b = 1 case
        M = 0; S = 0;
        for (int i = 0; i < H; i++) { S += a[i][j]; }
        dp[j][H][1] = M + S;
        for (int h = H-1; h >= 0; h--) {
            M = max(M, dp[j-1][h][0], dp[j-1][h][1]);
            S -= a[H-h-1][j-1];
            dp[j][h][1] = M + S;
        }
    }

    ll ans = 0;
    for (int h = 0; h <= H; h++) {
        ans = max(ans, dp[W][h][0]);
        ans = max(ans, dp[W][h][1]);
    }
    cout << ans << '\n';
}


/*
 * rooks
 */

#include <algorithm>
#include <iostream>
using namespace std;

typedef long long ll;

const int N = 1010;
int l[N];
ll dp[N][N];
const ll MOD = 1000*1000*1000+7;

int main (void) {
  int n;
  cin >> n;
  for (int i = 0; i < n; i++) { cin >> l[i]; }
  sort(l, l+n);

  dp[0][0] = 1;
  for (int i = 1; i <= n; i++)
    for (int j = 0; j <= k; j++) {
      dp[i][j] = dp[i-1][j]; // place no rooks in this row
      if (j > 0) { // or place a rook in this row
        dp[i][j] += dp[i-1][j-1] * (l[i-1] - (j-1));
        dp[i][j] %= MOD;
      }
    }
  
  cout << dp[n][k] << endl;
}

