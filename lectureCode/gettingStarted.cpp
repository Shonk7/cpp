/*
 *
 * code for gettingstarted
 *
 */

/*
 * sum 2
 */

#include <iostream>
#include <algorithm>
using namespace std;

const int N = 1001001;
int a[N];

int main() {
  // read input
  int n, k;
  cin >> n >> k;
  for (int i = 0; i < n; i++) { cin >> a[i]; }

  long long ret = 0, sum = 0;
  for (int i = 0; i < n; i++) {
    // remove a[i-k] if applicable
    if (i >= k) { sum -= a[i-k]; }
    // add a[i] to the window
    sum += a[i];

    // if a full window is formed, and it's the best so far, update
    if (i >= k - 1) { ret = max(ret, sum); }
  }

  // output the best window sum
  cout << ret << '\n';
}

/*
 * queens 1
 */

#include <iostream>
using namespace std;

int n, a[12];

void print_queens() {
  for (int k = 0; k < n; k++) {
    cout << a[k] + 1 << ' ';
  }
  cout << '\n';
}

bool check_queen (int i, int j) {
  for (int k = 0; k < i; k++) {
    if ((a[k] == j) || (i - k == a[k] - j) || (i - k == j - a[k])) {
      return false;
    }
  }
  return true;
}

/*
 * queens 2
 */

void go(int i) {
  if (i == n) {
    // we have placed all n queens legally, so print this solution
    print_queens();
    return;
  }

  for (int j = 0; j < n; j++) {
    // check whether a queen can be placed at (i,j)
    if (check_queen(i,j)) {
      // place queen and recurse
      a[i] = j;
      go(i+1);
    }
  }
}

int main() {
  cin >> n;
  go(0);
}

/*
 * count conflict
 */

#include<algorithm>
#include<cassert>
#include<iostream>
using namespace std;

// count nonzero array entries

const int N = 100100;
int a[N];

int main (void) {
  int n;
  cin >> n;
  
  int count = 0;
  for (int i = 0; i < n; i++) {
    cin >> a[i];
    if (a[i] != 0) { count++; }
  }

  // check our answer
  assert(count == n-count(a,a+n,0)); // conflict!

  cout << count << '\n';
}

/*
 * count no using
 */

#include<algorithm>
#include<cassert>
#include<iostream>

// count nonzero array entries

const int N = 100100;
int a[N];

int main (void) {
  int n;
  std::cin >> n;
  
  int count = 0;
  for (int i = 0; i < n; i++) {
    std::cin >> a[i];
    if (a[i] != 0) { count++; }
  }

  // check our answer
  std::assert(count == n-std::count(a,a+n,0)); // no conflict

  std::cout << count << '\n';
}

/*
 * count renamed
 */

#include<algorithm>
#include<cassert>
#include<iostream>
using namespace std;

// count nonzero array entries

const int N = 100100;
int a[N];

int main (void) {
  int n;
  cin >> n;
  
  int cnt = 0;
  for (int i = 0; i < n; i++) {
    cin >> a[i];
    if (a[i] != 0) { cnt++; }
  }

  // check our answer
  assert(cnt == n-count(a,a+n,0)); // no conflict

  cout << cnt << '\n';
}

/*
 * fast input
 */

cin.tie(nullptr); // prevents cout from flushing on every cin read
cin.sync_with_stdio(false); // unsyncs iostream from cstdio

/*
 * global array
 */

const int N = 100100;
int a[N];

/*
 * sodor
 */

#include <algorithm>
#include <iostream>
using namespace std;

const int N = 100100;
pair<string, int> engines[N]; // (name, original index)

int main (void) {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> engines[i].first;
        engines[i].second = i+1; // 1-based
    }

    sort(engines, engines + n); // sorts by increasing name
                                // and if necessary increasing index

    for (int i = 0; i < n; i++) {
        cout << engines[i].second << (i == n-1 ? '\n' : ' ');
    }
}

/*
 * next permutation
 */

int selected[n];
fill(selected,selected+n-k,0);
fill(selected+n-k,selected+n,1);

do {
    // ...
} while (next_permutation(selected,selected+n));
