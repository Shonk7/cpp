/*
 *
 * code for paradigms
 *
 */

/*
 * psychological
 */

#include <algorithm>
#include <iostream>
using namespace std;

const int N = 1010;
int opp[N];

int main() {
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++) { cin >> opp[i]; }
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        // draw against > i, lose to i, win against < i
        int cur = 0;
        for (int j = 1; j <= n; j++) {
            // if (opp[j] > i) { cur += 0; }
            if (opp[j] == i) { cur -= j; }
            if (opp[j] < i) { cur += j; }
        }
        ans = max(ans, cur);
    }
    cout << ans << '\n';
}

/*
 * stabbing
 */

#include <iostream>
#include <utility>
#include <algorithm>
using namespace std;

const int N = 1001001;
pair<int, int> victims[N];

int main() {
  // scan in intervals as (end, start) so as to sort by endpoint
  int n;
  cin >> n;
  for (int i = 0; i < n; i++) { cin >> victims[i].second >> victims[i].first; }
  sort(victims, victims + n);

  int last = -2000000001, res = 0;
  for (int i = 0; i < n; i++) {
    // if this interval has been stabbed already, do nothing
    if (victims[i].second <= last) { continue; }
    // otherwise stab at the endpoint of this interval
    res++;
    last = victims[i].first;
  }

  cout << res << '\n';
}

/*
 * restaurants
 */

#include <algorithm>
#include <iostream>
using namespace std;

const int N = 2020, M = 2020;
int numDelegates[N], numSeats[M];

int main() {
    int n, m;
    cin >> n;
    for (int i = 0; i < n; i++) { cin >> numDelegates[i]; }
    cin >> m;
    for (int j = 0; j < m; j++) { cin >> numSeats[j]; }

    int starved = 0;
    for (int i = 0; i < n; i++) {
        int delegatesRemaining = numDelegates[i];
        sort(numSeats, numSeats+m, greater<int>());
        for (int j = 0; j < m; j++) {
            if (numSeats[j] > 0 && delegatesRemaining > 0) {
                numSeats[j]--;
                delegatesRemaining--;
            }
        }
        starved += delegatesRemaining;
    }
    cout << starved << '\n';
}

/*
 * coordinate compression sort
 */

#include <algorithm>
#include <iostream>
#include <utility>
using namespace std;

const int N = 100100;
int values[N]; // assume filled
int cmprsd[N]; // filled by compress()
pair<int,int> helper[N]; // initially blank
int n;

void compress(void) {
    for (int i = 0; i < n; i++) { // (uncompressed coordinate, original index)
        helper[i].first = values[i];
        helper[i].second = i;
    }
    stable_sort(helper,helper+n); // sort by uncompressed coordinate
    for (int i = 0; i < n; i++) {
        // overwrite uncompressed coordinates with compressed
        helper[i].first = i;
        // unsort using original index for reverse lookup
        cmprsd[helper[i].second] = i;
        // warning: unequal compressed coordinates assigned to equal uncompressed coordinates
        // this may or may not be desirable
    }
}


/*
 * coordinate compression map
 */

#include <map>
using namespace std;

// coordinates -> (compressed coordinates).
map<int, int> compressed;

void compress(vector<int>& values) { // note reference
    for (int val : values) {
        compressed[val] = 0;
    }
    int cnt = 0;
    for (auto entry : compressed) {
        entry.second = cnt++;
    }
    for (int &val : values) { // note reference
        val = compressed[val];  // overwrites vector
    }
}


/*
 * binary search
 */

// Find the smallest X such that f(X) is true;
int binarysearch(function<bool(int)> f) {
    int lo = 0, hi = 100000, bestSoFar = -1;
    // Range [lo, hi];
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (f(mid)) {
            bestSoFar = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return bestSoFar;
}

/*
 * binary search library 1
 */

#include <algorithm>
#include <cassert>
#include <iostream>
using namespace std;

const int N = 100100;
int a[N];

int main () {
  int n;
  cin >> n;
  assert(n <= N);
  for (int i = 0; i < n; i++) { cin >> a[i]; }
  assert(is_sorted(a, a+n));

  int x;
  cin >> x;
  bool found = binary_search(a, a+n, x);
  cout << (found ? "found " : "did not find ") << x;


/*
 * binary search library 2
 */

  int y;
  cin >> y;
  int i = lower_bound(a, a+n, y) - a;
  if (i < n) {
    cerr << "a[" << i << "] = " << a[i] << " is the first entry to compare >= " << y;
  } else
    cerr << "all elements of a[] compare < " << y;
  }

  int z;
  cin >> z;
  int j = upper_bound(a,a+n,z) - a;
  if (j < n) {
    cerr << "a[" << j << "] = " << a[j] << " is the first entry to compare > " << z;
  } else {
    cerr << "all elements of a[] compare <= " << z;
  }
}


/*
 * chocolate bar 1
 */

#include <iostream>
using namespace std;

const int N = 1001001;
long long bar[N];
int n, k;

bool canDo(long long x) {
    long long curhap = 0;
    int numpcs = 0;
    for (int i = 0; i < n; i++) {
        curhap += bar[i];
        if (curhap >= x) {
            if (++numpcs == k) { // early exit
                return true;
			      }
            curhap = 0; // reset
        }
    }
    return false;
}


/*
 * chocolate bar 2
 */

int main() {
    cin >> n >> k;
    for (int i = 0; i < n; i++) { cin >> bar[i]; }
    long long lo = 1, hi = 1e11, ans = -1;
    while (lo <= hi) {
        long long mid = (lo + hi) / 2;
        // Trying to find the highest value that is feasible:
        if (canDo(mid)) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    cout << ans << '\n';
}


/*
 * robot moves 1
 */

#include <iostream>
using namespace std;

const int N = 200200;
int n, x;
string moves;
// delta[i] = -1 if L, 0 if S, 1 if R
// pre[i] = sum of first i moves
// suf[i] = sum of last i moves
int delta[N], pre[N], suf[N];

void precomp() {
    for (int i = 0; i < n; i++) {
        if (moves[i] == 'L') { delta[i] = -1; }
        else if (moves[i] == 'S') { delta[i] = 0; }
        else if (moves[i] == 'R') { delta[i] = 1; }
    }

    for (int i = 1; i <= n; i++) { pre[i] = pre[i-1] + delta[i-1]; }
    for (int i = 1; i <= n; i++) { suf[i] = suf[i-1] + delta[n-i]; }
}

/*
 * robot moves 2
 */

bool f (int m) {
    for (int i = 0; i+m <= n; i++) {
        // try replacing [i, i+m)
        int posAfterCut = pre[i] + suf[n-(i+m)];
        if (posAfterCut >= x-m && posAfterCut <= x+m) { return true; }
    }
    return false;
}

int main() {
    cin >> n >> x;
    cin >> moves;
    precomp();
    int lo = 0, hi = n, ans = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        // Trying to find the lowest value that is feasible:
        if (f(mid)) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    cout << ans << '\n';
}
