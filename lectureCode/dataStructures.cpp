/*
 *
 * code for datastructures
 *
 */

/*
 * vector
 */

#include <cassert>
#include <vector>
using namespace std;

int main() {
    vector<int> v;
    for (int i = 0; i < 10; i++) { v.push_back(i*2); }
    v[4] += 20;
    assert(v[4] == 28);
}

/*
 * stack queue
 */

#include <cassert>
#include <queue>
#include <stack>
using namespace std;

int main() {
    stack<int> stk;
    queue<long long> que;

    stk.push(1);
    stk.push(2);
    assert(stk.top() == 2);
    stk.pop();
    assert(stk.top() == 1);
    assert(stk.size() == 1);
    assert(!stk.empty());

    que.push(1);
    que.push(2);
    assert(que.front() == 1);
}

/*
 * set map
 */

#include <iostream>
#include <map>
#include <set>
using namespace std;

set<int> s;
map<int, char> m;

int main() {
    s.insert(2); s.insert(4); s.insert(1);
    m = {{1,'a'}, {4,'c'}, {2,'b'}};
    // Check membership: expect 1 0
    cout << (s.find(2) != s.end()) << ' ' << (s.find(3) != s.end()) << '\n';
    // NOT binary_search(s.begin(), s.end(), 2), which takes linear time

    // Access map:
    cout << m[1] << '\n'; // 'a'
    // WARNING: Access to non-existent data just silently adds it, avoid this.
    // cout << m[3] << '\n'; // null character

    // Lower and upper bounds:
    cout << *s.lower_bound(2) << '\n'; // 2
    // NOT *lower_bound(s.begin(), s.end(), 2), which takes linear time
    cout << *s.upper_bound(2) << '\n'; // 4
    auto it = m.lower_bound(2);
    cout << it->first << ' ' << it->second << '\n'; // 2 b

    // Move around with prev/next or increment/decrement
    cout << prev(it)->first << '\n'; // 1
    cout << (++it)->first << '\n'; // 4
}


/*
 * order statistics tree set
 */

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
using namespace std;

typedef tree<int, null_type, less<int>, rb_tree_tag, 
            tree_order_statistics_node_update> ordered_set;

ordered_set myset;
int main() {
    myset.insert(2);
    myset.insert(4);
    myset.insert(1);
    printf("%d\n", *(myset.find_by_order(0))); // 1
    printf("%d\n", (int)myset.order_of_key(3)); // 2
    printf("%d\n", (int)myset.order_of_key(4)); // 2
    printf("%d\n", (int)myset.size()); // 3
}

/*
 * order statistics tree map
 */

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
using namespace std;

typedef tree<int, char, less<int>, rb_tree_tag, 
            tree_order_statistics_node_update> ordered_map;

ordered_map mymap;
int main() {
    mymap[2] = 'a';
    mymap[4] = 'b';
    mymap[1] = 'c';
    pair<int, char> pic = *mymap.find_by_order(0);
    printf("%d %c\n", pic.first, pic.second); // 1 c
    printf("%d\n", (int)mymap.order_of_key(3)); // 2
    printf("%d\n", (int)mymap.order_of_key(4)); // 2
    printf("%d\n", (int)mymap.size()); // 3
}

/*
 * min heap
 */

#include <queue>
priority_queue <T, vector<T>, greater<T>> pq;

/*
 * restaurants with heap 1
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

const int N = 2020, M = 2020;
int numDelegates[N];
priority_queue<int> restaurants;

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) { cin >> numDelegates[i]; }
    
    int m;
    cin >> m;
    for (int i = 0; i < m; i++) {
        int s;
        cin >> s;
        restaurants.push(s);
    }


/*
 * restaurants with heap 2
 */

    int starved = 0;
    for (int i = 0; i < n; i++) {
        vector<int> poppedRestaurants;
        int delegatesRemaining = numDelegates[i];
        
        while (delegatesRemaining && !restaurants.empty()) {
            // seat a delegate at the restaurant with the most seats
            delegatesRemaining--;
            // remove this restaurant's capacity
            // to avoid seating multiple delegates here
            int seatsRemaining = restaurants.top();
            restaurants.pop();
            poppedRestaurants.push_back(seatsRemaining-1);
        }

        // only add back restaurants with capacity remaining
        // i.e. skip any that are now full
        for (int r : poppedRestaurants) {
            if (r > 0) { restaurants.push(r); }
        }
        
        // any unassigned delegates starve
        starved += delegatesRemaining;
    }
    cout << starved << '\n';
}


/*
 * sum 1
 */

#include <iostream>
#include <map>
using namespace std;

const int N = 100100;
int a[N];
int b[N];

int main() {
  int n, S;
  cin >> n;
  // read input and compute prefix sums
  for (int i = 0; i < n; i++) {
    cin >> a[i];
    b[i+1] = b[i] + a[i];
  }
  cin >> S;

  // answer could be up to 100,000 choose 2, approx 5e9
  long long ret = 0;
  map<int,int> freq;
  // freq[x] = k means that k of the prefix sums found so far equal x

/*
 * sum 2
 */

  // for each endpoint
  for (int j = 0; j <= n; j++) {
    /* each start point i satisfying b[i] = b[j] - S
       contributes 1 to the answer */
    /* if b[j] - S isn't already a key in the map
       it will be created with value 0, which is fine */
    ret += freq[b[j]-S];
    
    /* now add b[j] itself to the map
       as future endpoints should consider index j as a start point */
    freq[b[j]]++;
  }
  
  cout << ret << '\n';
}

/*
 * chairs 1
 */

#include <iostream>
#include <set>
using namespace std;

typedef set<int>::iterator si;

// which chairs are occupied
// initially only the endpoints
set<int> occupied;
// lengths of ranges between chairs
multiset<int> lengths;

// chairs between
inline int gap (si l, si r) { return *r - *l - 1; }


/*
 * chairs 2
 */

int main (void) {
  int M, U;
  cin >> M >> U;

  // dummy chairs at each end bound the extremal ranges
  occupied.insert(0);
  occupied.insert(M+1);
  lengths.insert(M);

  for (int i = 0; i < U; i++) {
    int c;
    cin >> c;

    // iterators to neighbours
    si r = occupied.upper_bound(c);
    si l = prev(r);
    // insert() returns pair of iterator and bool (?success)
    si p = occupied.insert(c).first;

    int old = gap(l,r), left = gap(l,p), right = gap(p,r);

    lengths.erase(lengths.find(old)); // erase one copy, not all
    lengths.insert(left); lengths.insert(right);

    cout << *lengths.rbegin() << '\n';
  }
}


/*
 * area under histogram 1
 */

#include <iostream>
#include <map>
using namespace std;

const int N = 100100;
int h[N];

int main() {
  int n;
  cin >> n;
  for (int i = 0; i < n; i++) { cin >> h[i]; }

  // left[i] is number of columns immediately left of i with height >= h[i]
  // how far left can you stretch bar i without going outside the histogram
  // function computes all left[i] values in O(n log n)
  vector<int> left = sweepleft();
  // left as an exercise, ideally without duplicating
  vector<int> right = sweepright();

  long long ans = 0;
  for (int i = 0; i < n; i++) {
    // area of rectangle formed by stretching bar i left and right
    long long cur = 1LL * (left[i] + 1 + right[i]) * h[i];
    ans = max(ans, cur);
  }
  cout << ans << '\n';
}


/*
 * area under histogram 2
 */

vector<int> sweepleft {
  vector<int> left;
  map<int, int> importantColumns; // height -> column index

  // dummy leftmost bar at index -1 with height -1
  // shorter than all real bars, so never deleted later
  importantColumns[-1] = -1;

  for (int i = 0; i < n; i++) {
    // find closest column to i's left with lower height
    // lower_bound finds first >= h[i]
    // so prev(lower_bound) finds last < h[i]
    // always exists because we inserted a dummy
    auto it = prev(importantColumns.lower_bound(h[i]));

    // left[i] counts columns strictly between this column and i
    left.push_back(i - it->second - 1);

    // some columns might no longer be important
    // as a result of column i being both later and shorter
    while ((int last = importantColumns.rbegin()->first) >= h[i]) {
      // m.erase(it) requires forward iterator; instead erase by key
      importantColumns.erase(last);
    }
    // column i is important (at least for now)
    importantColumns[h[i]] = i;
  }
  return left;
}


/*
 * union find slow
 */

int parent[N];

void init(int n) {
  for (int i = 0; i < n; i++)
    parent[i] = i;
}

int root(int x) {
  // only roots are their own parents
  return parent[x] == x ? x : root(parent[x]);
}

void join(int x, int y) {
  // join roots
  x = root(x); y = root(y);
  // test whether already connected
  if (x == y)
    return;
  parent[y] = x;
}

/*
 * union find size heuristic only
 */

int parent[N];
int subtree_size[N];  

void init(int n) {
  for (int i = 0; i < n; i++) {
    parent[i] = i;
    subtree_size[i] = 1;
  }
}

int root(int x) {
  // only roots are their own parents
  return parent[x] == x ? x : root(parent[x]);
}

void join(int x, int y) {
  // join roots
  x = root(x); y = root(y);
  // test whether already connected
  if (x == y)
    return;
  // size heuristic
  // hang smaller subtree under root of larger subtree
  if (subtree_size[x] < subtree_size[y]) {
    parent[x] = y;
    subtree_size[y] += subtree_size[x];
  } else {
    parent[y] = x;
    subtree_size[x] += subtree_size[y];
  }
}

/*
 * union find path compression
 */

int root(int x) {
  // only roots are their own parents
  // otherwise apply path compression
  return parent[x] == x ? x : parent[x] = root(parent[x]);
}

/*
 * union find incremental connectivity
 */

#include <iostream>
using namespace std;

// insert your union find implementation here
int root (int x);
void join (int x, int y);

int main() {
    int n, m;
    cin >> n >> m;
    for (int q = 0; q < m; q++) {
        char queryType;
        int a, b;
        cin >> queryType >> a >> b;
        if (queryType == 'U')
            join(a,b);
        else
            cout << (root(a) == root(b)) << '\n';
    }
    return 0;
}

/*
 * sparse tables precomp
 */

const int N = 100000;
const int LOGN = 18;

int a[N];
// sparseTable[l][i] = max a[i..i+2^l)
int sparseTable[LOGN][N];

void precomp(int n) {
    // level 0 is the array itself
    for (int i = 0; i < n; i++)
        sparseTable[0][i] = a[i];
    
    for (int l = 1; l < LOGN; l++) { // inner loop does nothing if 2^l > n
        int w = 1 << (l-1); // 2^(l-1)

        // a[i,i+2w) is made up of a[i,i+w) and a[i+w,i+2w)
        for (int i = 0; i + 2*w <= n; i++)
            sparseTable[l][i] = max(sparseTable[l-1][i],sparseTable[l-1][i+w]);
    }
}

/*
 * sparse tables query
 */

#include <algorithm>
#include <iostream>
using namespace std;

const int N = 100000, LOGN = 18;

int a[N], sparseTable[LOGN][N];
int log2s[N];

void precomp(int n);

int main() {
    // Input the initial array
    int n; cin >> n;
    for (int i = 0; i < n; i++)
        cin >> a[i];
    precomp(n);

    // log2s[i] = floor(log_2(i))
    for (int i = 2; i <= n; i++)
        log2s[i] = log2s[i/2] + 1;

    int q; cin >> q;
    for (int j = 0; j < q; j++) {
        int l, r; cin >> l >> r;
        // Problem: Find max of a[l,r)
        int lvl = log2s[r-l];
        cout << max(sparseTable[lvl][l], sparseTable[lvl][r-(1<<lvl)]) << '\n';
    }
}

/*
 * range tree query
 */

const int N = 100100;
// N leaf nodes plus
// internal nodes, up to the next power of two up from N (2^17 = 131,072)
int tree[1<<18];

int n; // actual length of underlying array

// query the sum over [qL, qR) (0-based)
// i is the index in the tree, rooted at 1 so children are 2i and 2i+1
// instead of explicitly storing each node's range of responsibility [cL,cR), we calculate it on the way down
// the root node is responsible for [0, n)
int query(int qL, int qR, int i = 1, int cL = 0, int cR = n) {
  // the query range exactly matches this node's range of responsibility
  if (cL == qL && cR == qR)
    return tree[i];
  // we might need to query one or both of the children
  int mid = (cL + cR) / 2;
  int ans = 0;
  // query the part within the left child [cL, mid), if any
  if (qL < mid) ans += query(qL, min(qR, mid), i * 2, cL, mid);
  // query the part within the right child [mid, cR), if any
  if (qR > mid) ans += query(max(qL, mid), qR, i * 2 + 1, mid, cR);
  return ans;
}


/*
 * range tree update
 */

// p is the index in the array (0-based)
// v is the value that the p-th element will be updated to
// i is the index in the tree, rooted at 1 so children are 2i and 2i+1
// instead of explicitly storing each node's range of responsibility [cL,cR), we calculate it on the way down
// the root node is responsible for [0, n)
void update(int p, int v, int i = 1, int cL = 0, int cR = n) {
  if (cR - cL == 1) {
    // this node is a leaf, so apply the update
    tree[i] = v;
    return;
  }
  // figure out which child is responsible for the index (p) being updated
  int mid = (cL + cR) / 2;
  if (p < mid)
    update(p, v, i * 2, cL, mid);
  else
    update(p, v, i * 2 + 1, mid, cR);
  // once we have updated the correct child, recalculate our stored value.
  tree[i] = tree[i*2] + tree[i*2+1];
}

/*
 * range tree debug
 */

// print the entire tree to stderr
// instead of explicitly storing each node's range of responsibility [cL,cR), we calculate it on the way down
// the root node is responsible for [0, n)
void debug(int i = 1, int cL = 0, int cR = n) {
  // print current node's range of responsibility and value
  cerr << "tree[" << cL << "," << cR << ") = " << tree[i] << endl;
  
  if (cR - cL > 1) { // not a leaf
    // recurse within each child
    int mid = (cL + cR) / 2;
    debug(i * 2, cL, mid);
    debug(i * 2 + 1, mid, cR);
  }
}


/*
 * lis with rt
 */

#include <algorithm>
#include <iostream>
using namespace std;

const int N = 100100;
int tree[1<<18];

// range max tree over array values (not indices)
// note: root covers [0,N) not [0,n)
int query(int qL, int qR, int i = 1, int cL = 0, int cR = N);
void update(int p, int v, int i = 1, int cL = 0, int cR = N);

int main() {
  int n;
  cin >> n;

  for (int i = 0; i < n; i++) {
    int x;
    cin >> x;
    int best = 1 + query(0, x);
    update(x, best);
  }
  
  cout << query(0, N) << '\n';
}

/*
 * card trick
 */

#include <iostream>
using namespace std;

const int N = 100100;
int tree[1<<18];

int n;

// range sum tree
int query(int qL, int qR, int i = 1, int cL = 0, int cR = n);
void update(int p, int v, int i = 1, int cL = 0, int cR = n);

int main() {
  int q;
  for (int i = 0; i < q; i++) {
    char type;
    cin >> type;
    if (type == 'F') {
      int i, j;
      cin >> i >> j;
      update(i-1, 1);
      update(j, -1);
    }
    else if (type == 'Q') {
      int k;
      cin >> k;
      cout << ((query(0, k) % 2) ? "Face up\n" : "Face down\n");
    }
  }
}

/*
 * range tree on trees
 */

#include <vector>
using namespace std;

const int N = 100100;
// Suppose you already have your tree set up.
vector<int> children[N];
// A node is responsible for the range [startRange[v], endRange[v])
int indexInRangeTree[N], startRange[N], endRange[N];
int totId;
// A range tree that supports point update, range sum queries.
long long rangeTree[1<<18];
void update(int plc, int v);
long long query(int qL, int qR); // Query for [qL, qR)

void compute_tree_ranges(int c) {
    indexInRangeTree[c] = startRange[c] = totId++;
    for (int nxt : children[c]) {
        compute_tree_ranges(nxt);
    }
    endRange[c] = totId;
}

void update_node(int id, int v) {
    update(indexInRangeTree[id], v);
}

int query_subtree(int id) {
    return query(startRange[id], endRange[id]);
}

/*
 * maximum sum subrange 1
 */

using namespace std;

const int MAXN = 100100;

struct state {
  long long bestStart, bestEnd, sum, bestSubarray;
};

// Default value of state is all 0. This is correct for us.
state rt[1<<18];

state mergeStates(const state& left, const state& right) {
  state ret;
  ret.bestStart = max(left.bestStart, left.sum + right.bestStart);
  ret.bestEnd = max(right.bestEnd, left.bestEnd + right.sum);
  ret.sum = left.sum + right.sum;
  ret.bestSubarray = max(max(left.bestSubarray, right.bestSubarray),
      left.bestEnd + right.bestStart);
  /* in C++11, can instead do ret.bestSubarray = max({left.bestSubarray,
      right.bestSubarray, left.bestEnd + right.bestStart}) */
  return ret;
}

/*
 * maximum sum subrange 2
 */

int n;

void update(int p, int v, int i=1, int cL = 0, int cR = n) {
  if (cR - cL == 1) {
    rt[i].sum = v;
    rt[i].bestStart = rt[i].bestEnd = rt[i].bestSubarray = max(v,0);
    return;
  }
  int mid = (cL + cR) / 2;
  if (p < mid) update(p, v, i * 2, cL, mid);
  else update(p, v, i * 2 + 1, mid, cR);
  rt[i] = mergeStates(rt[i*2], rt[i*2+1]);
}

state query(int qL, int qR, int i = 1, int cL = 0, int cR = n) {
  if (qL == cL && qR == cR) {
    return rt[i];
  }
  int mid = (cL + cR) / 2;
  if (qR <= mid) return query(qL, qR, i * 2, cL, mid);
  if (qL >= mid) return query(qL, qR, i * 2 + 1, mid, cR);
  return mergeStates(
      query(qL, min(qR, mid), i * 2, cL, mid),
      query(max(qL, mid), qR, i * 2 + 1, mid, cR));
}

/*
 * bit sum
 */

// computes the sum up to and including i
int prefix_sum(int i) {
  int ret = 0;
  while (i > 0) {
    ret += t[i];
    i -= i & (-i);
  }
  return ret;
}

/*
 * bit update
 */

// increase a[i] by k
void update(int i, int k) {
  while (i <= n) {
    t[i] += k;
    i += i & (-i);
  }
}

/*
 * bit scaling
 */

void scale(int c) {
  for (int i = 1; i <= n; i++) { t[i] *= c; }
}
