// Set1-B

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
using namespace std;
typedef long long ll;

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int t, n;
    ll x;
    cin >> t;

    while (t--) {
        cin >> n;
        ll ans = 0;
        ll groupMax = LLONG_MIN;
        bool first = true;
        bool sign;

        while (n--) {
            cin >> x;
            bool xSign = (x > 0);
            if (first || xSign == sign) {
                groupMax = max(groupMax, x);
            } else {
                ans += groupMax;
                groupMax = x;
            }
            sign = xSign;
            first = false;
        }
        ans += groupMax;
        cout << ans << '\n';
    }

    return 0;
}
