// Set1-A

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
using namespace std;
// shortcut pretty useful 
typedef long long ll;

int main() {
    // saw on last years course notes i need this?
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int n;
    ll a, p;
    cin >> n;

    ll total = 0;
    ll minP = INT_MAX;
    while (n--) {
        cin >> a >> p;
        // get optimal price
        minP = min(minP, p);
        total += minP * a;
    }

    cout << total << '\n';

    return 0;
}
