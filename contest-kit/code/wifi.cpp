#include <bits/stdc++.h>
using namespace std;

#define i32 int32_t
#define int long long
#define uint unsigned long long

#define fastio        \
    cin.tie(nullptr); \
    cin.sync_with_stdio(false);

#define all(c) c.begin(), c.end()
#define space << " " <<
#define newline << "\n"

using namespace std;
const int INF = 1e9; 

struct Node {
    int min;
};
const int N = 100100;
Node rt[1 << 18];
int n;

Node merge(const Node& L, const Node& R) {
    Node ret;
    ret.min = min(L.min, R.min);
    return ret;
}

void update(int p, int v, int i = 1, int cL = 0, int cR = n) {
    // if node is a leaf
    if (cR - cL == 1) {
        rt[i] = {v};
        return;
    }
    int mid = (cL + cR) / 2;
    // need to recursively update both sides
    if (p < mid) {
        update(p, v, 2*i, cL, mid);
    } else {
        update(p, v, 2*i + 1, mid, cR);
    }
    // after updating children, merge them
    rt[i] = merge(rt[2*i], rt[2*i + 1]);
}

Node query(int qL, int qR, int i = 1, int cL = 0, int cR = n) {
    if (qL >= qR) return {INF};
    // if query matches current range
    if (qL == cL && qR == cR) return rt[i];

    int mid = (cL + cR) / 2;
    // query is entirely in left child
    if (qR <= mid) return query(qL, qR, 2*i, cL, mid);
    // query is entirely in right child
    if (qL >= mid) return query(qL, qR, 2*i + 1, mid, cR);

    return merge(query(qL, min(qR, mid), 2*i, cL, mid), query(max(qL, mid), qR, 2*i + 1, mid, cR));

}
// each end point has a vector which stores ther pairs (segmnets - routers which end their - sotring their start and their cost)
vector<pair<int, int>> segments[N];
int dp[N];
i32 main () {
    int k;
    cin >> n >> k;
    string s;
    cin >> s;
    for (int i = 1; i <= n; i++) {
        // end - start + cost
        segments[i].emplace_back(i, i);
        if (s[i - 1] == '1') segments[min(n, i + k)].emplace_back(max((int)1, i - k), i);
    }
    // end positions
    for (int i = 1; i  <= n; i++) {
        dp[i] = INF;
        for (auto seg : segments[i]) {
            int prevcost = seg.first == 1 ? 0 : query(seg.first - 1, i).min;
            dp[i] = min(dp[i], prevcost + seg.second);
        }
        update(i, dp[i]);
    }

    cout << dp[n] newline;

    return 0;
}
