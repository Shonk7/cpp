#pragma region

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#define int long long
#define uint unsigned long long

#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define ii int, int
#define vec vector
#define uset unordered_set
#define umap unordered_map
#define mii map<ii>
#define umii umap<ii>
#define pq priority_queue
#define pii pair<ii>
#define vi vec<int>
#define vii vec<pii>
#define vvi vec<vec<int>>
#define vvii vec<vec<pii>>

#define all(c) c.begin(), c.end()
#define sign(x) (x > 0 ? 1 : (x < 0 ? -1 : 0))

#define __NARG__(...) __NARG_I_(__VA_ARGS__, __RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define __RSEQ_N() 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__))(__VA_ARGS__)

#define range(...) VFUNC(range, __VA_ARGS__)
#define rangei(...) VFUNC(rangei, __VA_ARGS__)
#define rrange(...) VFUNC(rrange, __VA_ARGS__)
#define get(...) VFUNC(get, __VA_ARGS__)

#define range3(i, x, n) for (int i = x; i < n; i++)
#define range4(t, i, x, n) for (t i = x; i < n; i++)
#define rangei3(i, x, n) for (int i = x; i <= n; i++)
#define rangei4(t, i, x, n) for (t i = x; i <= n; i++)
#define rrange3(i, x, n) for (int i = x; i >= n; i--)
#define rrange4(t, i, x, n) for (t i = x; i >= n; i--)
#define get1(i) \
	int i;      \
	cin >> i;
#define get2(t, i) \
	t i;           \
	cin >> i;

#define space << " "
#define newline << "\n"

#define bcount __builtin_popcountll
#define bset0(bs, k) ((bs) & ~(1 << (k)))
#define bset1(bs, k) ((bs) | (1 << (k)))
#define bset(bs, k, n) (n == 0 ? bset0(bs, k) : bset1(bs, k))
#define btog(bs, k) ((bs) ^ (1 << k))
#define bget(bs, k) ((bs) & (1 << k))

#define fastio        \
	cin.tie(nullptr); \
	cin.sync_with_stdio(false);

using namespace std;

template <typename A, typename B>
ostream& operator<<(ostream& os, const pair<A, B>& p) {
	return os << '(' << p.first << ", " << p.second << ')';
}
template <
	typename T_container,
	typename T = typename enable_if<
		!is_same<T_container, string>::value,
		typename T_container::value_type>::type>
ostream& operator<<(ostream& os, const T_container& v) {
	os << '{';
	string sep;
	for (const T& x : v) os << sep << x, sep = ", ";
	return os << '}';
}

#pragma endregion

struct Query {
	char type;
	int a;
	int b;
	int prev_value;
};

struct DSU {
	vi parent, size, comp_pop;
	multiset<int> pops;

	DSU() = default;

	DSU(int n, const vi& node_pop)
		: parent(n + 1), size(n + 1, 1), comp_pop(n + 1, 0) {
		rangei(i, 1, n) {
			parent[i] = i;
			comp_pop[i] = node_pop[i];
			pops.insert(comp_pop[i]);
		}
	}

	int find(int x) {
		if (parent[x] == x) return x;
		return parent[x] = find(parent[x]);
	}

	void erase_comp_pop(int value) {
		auto it = pops.find(value);
		if (it != pops.end()) pops.erase(it);
	}

	void unite(int a, int b) {
		a = find(a);
		b = find(b);
		if (a == b) return;
		if (size[a] < size[b]) swap(a, b);

		erase_comp_pop(comp_pop[a]);
		erase_comp_pop(comp_pop[b]);

		parent[b] = a;
		size[a] += size[b];
		comp_pop[a] += comp_pop[b];
		pops.insert(comp_pop[a]);
	}

	void update_node_population(int node, int delta) {
		int root = find(node);
		erase_comp_pop(comp_pop[root]);
		comp_pop[root] += delta;
		pops.insert(comp_pop[root]);
	}

	int max_population() const {
		return *pops.rbegin();
	}
};

i32 main() {
	fastio;

	int n, m, q;
	cin >> n >> m >> q;

	vi initial_pop(n + 1);
	rangei(i, 1, n) cin >> initial_pop[i];

	vii edges(m + 1);
	rangei(i, 1, m) {
		int x, y;
		cin >> x >> y;
		edges[i] = {x, y};
	}

	vec<Query> queries(q);
	vec<bool> deleted(m + 1, false);
	range(i, 0, q) {
		char type;
		cin >> type;
		if (type == 'D') {
			int idx;
			cin >> idx;
			queries[i] = {type, idx, 0, 0};
			deleted[idx] = true;
		} else {
			int city, new_pop;
			cin >> city >> new_pop;
			queries[i] = {type, city, new_pop, 0};
		}
	}

	vi current_pop = initial_pop;
	range(i, 0, q) {
		if (queries[i].type == 'P') {
			int city = queries[i].a;
			queries[i].prev_value = current_pop[city];
			current_pop[city] = queries[i].b;
		}
	}

	DSU dsu(n, current_pop);
	rangei(i, 1, m) {
		if (!deleted[i]) dsu.unite(edges[i].first, edges[i].second);
	}

	vi answer_rev;
	answer_rev.reserve(q);

	rrange(i, q - 1, 0) {
		answer_rev.push_back(dsu.max_population());

		const Query& query = queries[i];
		if (query.type == 'D') {
			int edge_idx = query.a;
			dsu.unite(edges[edge_idx].first, edges[edge_idx].second);
		} else {
			int city = query.a;
			int delta = query.prev_value - query.b;
			dsu.update_node_population(city, delta);
		}
	}

	reverse(all(answer_rev));
	for (int value : answer_rev) cout << value newline;

	return 0;
}
