#include <bits/stdc++.h>
using namespace std;

#define i32 int32_t
#define int long long

#define fastio        \
    cin.tie(nullptr); \
    cin.sync_with_stdio(false);

const int N = 500000 + 5;
const int SEG = 1 << 22;

int n;
vector<int> adj[N];
int tin[N], tout[N], par[N], ptrIdx[N], timerCnt = 0;

int fillLazy[SEG];
int emptyRt[SEG];

void fillUpdate(int qL, int qR, int v, int i = 1, int cL = 0, int cR = n) {
    if (qL >= cR || qR <= cL) return;
    if (qL <= cL && cR <= qR) {
        fillLazy[i] = v;
        return;
    }

    int mid = (cL + cR) / 2;
    fillUpdate(qL, qR, v, 2 * i, cL, mid);
    fillUpdate(qL, qR, v, 2 * i + 1, mid, cR);
}

int fillQuery(int p, int i = 1, int cL = 0, int cR = n) {
    if (cR - cL == 1) return fillLazy[i];

    int mid = (cL + cR) / 2;
    if (p < mid) return max(fillLazy[i], fillQuery(p, 2 * i, cL, mid));
    return max(fillLazy[i], fillQuery(p, 2 * i + 1, mid, cR));
}

void emptyUpdate(int p, int v, int i = 1, int cL = 0, int cR = n) {
    if (cR - cL == 1) {
        emptyRt[i] = v;
        return;
    }

    int mid = (cL + cR) / 2;
    if (p < mid) emptyUpdate(p, v, 2 * i, cL, mid);
    else emptyUpdate(p, v, 2 * i + 1, mid, cR);

    emptyRt[i] = max(emptyRt[2 * i], emptyRt[2 * i + 1]);
}

int emptyQuery(int qL, int qR, int i = 1, int cL = 0, int cR = n) {
    if (qL >= cR || qR <= cL) return 0;
    if (qL <= cL && cR <= qR) return emptyRt[i];

    int mid = (cL + cR) / 2;
    return max(emptyQuery(qL, qR, 2 * i, cL, mid),
               emptyQuery(qL, qR, 2 * i + 1, mid, cR));
}

void buildEuler() {
    vector<int> st;
    st.reserve(n);
    st.push_back(1);
    par[1] = -1;

    while (!st.empty()) {
        int u = st.back();
        if (ptrIdx[u] == 0) tin[u] = timerCnt++;

        if (ptrIdx[u] < (int)adj[u].size()) {
            int v = adj[u][ptrIdx[u]++];
            if (v == par[u]) continue;
            par[v] = u;
            st.push_back(v);
        } else {
            tout[u] = timerCnt;
            st.pop_back();
        }
    }
}

i32 main() {
    fastio;

    cin >> n;
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    buildEuler();

    int q;
    cin >> q;
    for (int t = 1; t <= q; t++) {
        int type, v;
        cin >> type >> v;

        if (type == 1) {
            fillUpdate(tin[v], tout[v], t);
        } else if (type == 2) {
            emptyUpdate(tin[v], t);
        } else {
            int lastFill = fillQuery(tin[v]);
            int lastEmpty = emptyQuery(tin[v], tout[v]);
            cout << (lastFill > lastEmpty ? 1 : 0) << '\n';
        }
    }

    return 0;
}
