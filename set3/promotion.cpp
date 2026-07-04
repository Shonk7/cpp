#include <bits/stdc++.h>
using namespace std;

#define i32 int32_t
#define int long long
#define uint unsigned long long

#define fastio        \
    cin.tie(nullptr); \
    cin.sync_with_stdio(false);

#define all(c) c.begin(), c.end()

using namespace std;

struct Node {
    int sum;
};
const int N = 100100;
Node rt[1 << 18];
int n;

vector<int> child[N];
int tin[N], tout[N], timer = 0;
int ans[N];

Node merge(const Node& L, const Node& R) {
    return {L.sum + R.sum};
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
    // if query matches current range
    if (qL == cL && qR == cR) return rt[i];

    int mid = (cL + cR) / 2;
    // query is entirely in left child
    if (qR <= mid) return query(qL, qR, 2*i, cL, mid);
    // query is entirely in right child
    if (qL >= mid) return query(qL, qR, 2*i + 1, mid, cR);

    return merge(query(qL, min(qR, mid), 2*i, cL, mid),
     query(max(qL, mid), qR, 2*i + 1, mid, cR));
    
}

// gets euler walk indices 
// store tree flatteign in two arrays
// [0, 6) inclusive on left exclusive on right
void dfs(int u) {
    tin[u] = timer++;

    for (auto chi : child[u]) {
        dfs(chi);
    }

    tout[u] = timer;
}

i32 main() {
    fastio;
    freopen("promote.in","r",stdin);
    freopen("promote.out","w",stdout);
    
    cin >> n;
    vector<int> p(n + 1);
    vector<int> c(n);

    // cows and p levels will sort later
    for (int i = 1; i <= n; i++) {
        cin >> p[i];
        c[i - 1] = i; 
    }

    // get children 
    for (int i = 2; i <= n; i++) {
        int parent;
        cin >> parent;
        child[parent].push_back(i);

    }

    // euler walk to work out the ranges of children
    dfs(1);

    // sort cows by proficency we want to do the highest cows first
    sort(all(c), [&](int a, int b) {
        return p[a] > p[b];
    });

    for (auto cow : c) {
        ans[cow] = query(tin[cow], tout[cow]).sum;
        // if this cow appears in a query it has to have had a higher profficency of the proceeding
        update(cow, 1);
    }

    for (int i = 1; i <= n; i++) {
        cout << ans[i] << '\n';
    }
    return 0;
}
