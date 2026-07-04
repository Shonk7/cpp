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

struct Node {
    int sum;
};
const int N = 100100;
Node rt[1 << 18];


vector<int> child[N];
int tin[N], tout[N], timer = 0;
int ans[N];

Node merge(const Node& L, const Node& R) {
    return {L.sum + R.sum};
}

struct UF {
    vector<int> par, sz;
    UF(int n) : par(n), sz(n, 1) { 
        for (int i = 0; i < n; i ++) par[i] = i; 
    }
    int find(int x) { return par[x] == x ? x : par[x] = find(par[x]); }
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (sz[a] < sz[b]) swap(a, b);
        par[b] = a; sz[a] += sz[b];
    }
};

int q;
i32 main() {
    fastio;

    int n;
    cin >> n >> q;
    UF uf(n + 10);
    int t, x, y;
    set<pair<int, int>> ranges;
    for (int i = 1; i <= n; i++) {
        ranges.insert({i, i});
    }
    while (q--) {
        cin >> t >> x >> y;
        // union join
        if (t == 1) {
            uf.unite(x, y);
        } 
        //merge ranges and union join
        else if (t == 2) {
            // find range with x
            auto it = ranges.upper_bound({x, 1e9});
            it--;

            // need to merge with all the rnages after x which are less than y
            vector<set<pair<int, int>>::iterator> toMerge;
            while (it->second <= y) {
                toMerge.push_back(it);
                it++;
            }

            // now to merge
            int newS = toMerge.front()->first;
            int newE = toMerge.back()->second;
            // when mergin we run union find
            for (auto it : toMerge) {
                uf.unite(it->first, newS);
                ranges.erase(it);
            }

            ranges.insert({newS, newE});
        }
        // query
        else {
            if (uf.find(x) == uf.find(y)) {
                cout << "YES" newline;
            } else {
                cout << "NO" newline;
            }
        }
    }

    return 0;
}