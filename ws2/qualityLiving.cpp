#include <bits/stdc++.h>
using namespace std;

// Returns true if there exists an H x W rectangle whose median is <= x.
static bool feasibleMedian(int x, int R, int C, int H, int W,
                           const vector<vector<int>>& Q,
                           vector<int>& pref) {
    const int stride = C + 1;

    for (int j = 0; j <= C; ++j) pref[j] = 0;
    for (int i = 1; i <= R; ++i) {
        pref[i * stride] = 0;
        int rowBase = i * stride;
        int prevRowBase = (i - 1) * stride;
        const vector<int>& row = Q[i - 1];

        for (int j = 1; j <= C; ++j) {
            int v = (row[j - 1] > x ? 1 : -1);
            pref[rowBase + j] = v + pref[prevRowBase + j] + pref[rowBase + j - 1] - pref[prevRowBase + j - 1];
        }
    }

    for (int i = H; i <= R; ++i) {
        int top = i - H;
        int iBase = i * stride;
        int topBase = top * stride;
        for (int j = W; j <= C; ++j) {
            int left = j - W;
            int sum = pref[iBase + j] - pref[topBase + j] - pref[iBase + left] + pref[topBase + left];
            if (sum <= 0) return true;
        }
    }
    return false;
}

int rectangle(int R, int C, int H, int W, const vector<vector<int>>& Q) {
    int lo = 1;
    int hi = R * C;
    int ans = hi;

    vector<int> pref((R + 1) * (C + 1), 0);

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (feasibleMedian(mid, R, C, H, W, Q, pref)) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C, H, W;
    if (!(cin >> R >> C >> H >> W)) return 0;

    vector<vector<int>> Q(R, vector<int>(C));
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            cin >> Q[i][j];
        }
    }

    cout << rectangle(R, C, H, W, Q) << '\n';
    return 0;
}
