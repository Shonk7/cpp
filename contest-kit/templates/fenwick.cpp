#include <bits/stdc++.h>
using namespace std;
#define int long long

const int N = 100100;
int bit[N];
int n;

void update(int i, int delta) {
    for (; i <= n; i += i & (-i))
        bit[i] += delta;
}

int query(int i) {
    int sum = 0;
    for (; i > 0; i -= i & (-i))
        sum += bit[i];
    return sum;
}

int range_query(int l, int r) {
    return query(r) - query(l - 1);
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 1; i <= n; i++) {
        int x;
        cin >> x;
        update(i, x);
    }

    int q;
    cin >> q;
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << range_query(l, r) << "\n";
    }

    return 0;
}
