#include <bits/stdc++.h>
using namespace std;
#define int long long

const int INF = 1e9;
const int N = 100100;

struct Node {
    int minval;  // change based on problem
};

Node rt[1 << 18];
int n;

Node merge(const Node& L, const Node& R) {
    return {min(L.minval, R.minval)};
}

void update(int p, int v, int i = 1, int cL = 0, int cR = n) {
    if (cR - cL == 1) {
        rt[i] = {v};
        return;
    }
    int mid = (cL + cR) / 2;
    if (p < mid) update(p, v, 2*i, cL, mid);
    else update(p, v, 2*i + 1, mid, cR);
    rt[i] = merge(rt[2*i], rt[2*i + 1]);
}

Node query(int qL, int qR, int i = 1, int cL = 0, int cR = n) {
    if (qL >= qR) return {INF};
    if (qL == cL && qR == cR) return rt[i];
    int mid = (cL + cR) / 2;
    if (qR <= mid) return query(qL, qR, 2*i, cL, mid);
    if (qL >= mid) return query(qL, qR, 2*i + 1, mid, cR);
    return merge(query(qL, min(qR, mid), 2*i, cL, mid),
                 query(max(qL, mid), qR, 2*i + 1, mid, cR));
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        update(i, x);
    }

    int q;
    cin >> q;
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << query(l, r).minval << "\n";
    }

    return 0;
}
