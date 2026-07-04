// Set1-D

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <queue>
using namespace std;
typedef long long ll;

bool canK(vector<ll>& arr, ll n, ll c, ll d, ll k) {
    ll cycleLen = k + 1;
    // if n is smaller than cycleLen then we can only loop through those elements or memory error
    ll take = min(n, cycleLen);

    ll cycleSum = 0;
    for (ll i = 0; i < take; i++) {
        cycleSum += arr[i];
    }
    // work out how many cycles + rem we can do in d days
    ll fullCycles = d / cycleLen;
    ll rem = d % cycleLen;

    ll total = fullCycles * cycleSum;
    for (ll i = 0; i < min(take, rem); i++) {
        total += arr[i];
    }

    return total >= c;
}

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    ll t, n, c, d;

    cin >> t;
    while (t--) {
        cin >> n >> c >> d;
        // get vector
        vector<ll> arr;
        for (int i = 0;  i < n; i ++) {
            ll x;
            cin >> x;
            arr.push_back(x);
        }
        // sort
        sort(arr.begin(), arr.end(), greater<ll>());

        // check impossible
        ll maxSum = 0;
        for (int i = 0; i < d; i++) {
            maxSum += arr[0];
        }
        if (maxSum < c) {
            cout << "Impossible" << "\n";
            continue;
        }

        // check infinity
        maxSum = 0;
        for (int i = 0; i < min(n, d); i++) {
            maxSum += arr[i];
        }

        if (maxSum >= c) {
            cout << "Infinity" << "\n";
            continue;
        }

        ll lo = 0;
        ll hi = d;
        ll ans = 0;
        while (lo <= hi) {
            ll mid = lo + (hi - lo) / 2;
            if (canK(arr, n, c, d, mid)) {
                ans = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        cout << ans << "\n";
    }


    return 0;
}
