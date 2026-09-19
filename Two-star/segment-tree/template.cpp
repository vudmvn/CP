/**
 * @file template.cpp
 * @brief Production-Ready Segment Tree (PURQ, Lazy RURQ, Walk on Tree, Dynamic Segment Tree)
 * @author Duc-Minh Vu
 * @institution SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), National Economics University (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * 
 * Standard C++20 implementations for:
 * 1. PointSegmentTree<Info>: Fast Point Update - Range Query Segment Tree.
 * 2. LazySegmentTree<Info, Tag>: Production Range Update - Range Query with Lazy Propagation.
 * 3. Walk on Segment Tree: In-tree binary search in O(log N).
 * 4. DynamicSegmentTree: Implicit node allocation for coordinate range up to 10^18.
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <cstdint>
#include <functional>
#include <cassert>
#include <algorithm>

namespace cp::data_structures {

// =========================================================================
// 1. Point Update - Range Query Segment Tree (PURQ)
// =========================================================================
template <typename Info>
class PointSegmentTree {
private:
    int n;
    std::vector<Info> tree;

    void pull(int id) {
        tree[id] = Info::combine(tree[2 * id], tree[2 * id + 1]);
    }

    void build(int id, int l, int r, const std::vector<Info>& init) {
        if (l == r) {
            tree[id] = init[l];
            return;
        }
        int mid = (l + r) / 2;
        build(2 * id, l, mid, init);
        build(2 * id + 1, mid + 1, r, init);
        pull(id);
    }

    void update(int id, int l, int r, int pos, const Info& val) {
        if (l == r) {
            tree[id] = val;
            return;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) update(2 * id, l, mid, pos, val);
        else update(2 * id + 1, mid + 1, r, pos, val);
        pull(id);
    }

    Info query(int id, int l, int r, int ql, int qr) const {
        if (ql <= l && r <= qr) return tree[id];
        int mid = (l + r) / 2;
        if (qr <= mid) return query(2 * id, l, mid, ql, qr);
        if (ql > mid) return query(2 * id + 1, mid + 1, r, ql, qr);
        return Info::combine(query(2 * id, l, mid, ql, qr), query(2 * id + 1, mid + 1, r, ql, qr));
    }

public:
    PointSegmentTree() : n(0) {}

    explicit PointSegmentTree(int size) : n(size), tree(4 * size + 4) {}

    explicit PointSegmentTree(const std::vector<Info>& init)
        : n(static_cast<int>(init.size()) - 1), tree(4 * init.size() + 4) {
        if (n >= 1) build(1, 1, n, init);
    }

    void update(int pos, const Info& val) {
        assert(1 <= pos && pos <= n);
        update(1, 1, n, pos, val);
    }

    Info query(int ql, int qr) const {
        assert(1 <= ql && ql <= qr && qr <= n);
        return query(1, 1, n, ql, qr);
    }
};

// =========================================================================
// 2. Production Lazy Segment Tree (RURQ & Walk on Tree)
// =========================================================================
template <typename Info, typename Tag>
class LazySegmentTree {
private:
    int n;
    std::vector<Info> tree;
    std::vector<Tag> lazy;

    void pull(int id) {
        tree[id] = Info::combine(tree[2 * id], tree[2 * id + 1]);
    }

    void apply_tag(int id, int l, int r, const Tag& tag) {
        tree[id].apply(tag, l, r);
        lazy[id].apply(tag);
    }

    void push(int id, int l, int r) {
        if (!lazy[id].has_update()) return;
        int mid = (l + r) / 2;
        apply_tag(2 * id, l, mid, lazy[id]);
        apply_tag(2 * id + 1, mid + 1, r, lazy[id]);
        lazy[id].clear();
    }

    void build(int id, int l, int r, const std::vector<Info>& init) {
        if (l == r) {
            tree[id] = init[l];
            return;
        }
        int mid = (l + r) / 2;
        build(2 * id, l, mid, init);
        build(2 * id + 1, mid + 1, r, init);
        pull(id);
    }

    void update(int id, int l, int r, int ql, int qr, const Tag& tag) {
        if (ql <= l && r <= qr) {
            apply_tag(id, l, r, tag);
            return;
        }
        push(id, l, r);
        int mid = (l + r) / 2;
        if (ql <= mid) update(2 * id, l, mid, ql, qr, tag);
        if (qr > mid) update(2 * id + 1, mid + 1, r, ql, qr, tag);
        pull(id);
    }

    Info query(int id, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tree[id];
        push(id, l, r);
        int mid = (l + r) / 2;
        if (qr <= mid) return query(2 * id, l, mid, ql, qr);
        if (ql > mid) return query(2 * id + 1, mid + 1, r, ql, qr);
        return Info::combine(query(2 * id, l, mid, ql, qr), query(2 * id + 1, mid + 1, r, ql, qr));
    }

    template <typename Pred>
    int find_first(int id, int l, int r, int ql, int qr, Pred&& pred) {
        if (l > qr || r < ql || !pred(tree[id])) return -1;
        if (l == r) return l;
        push(id, l, r);
        int mid = (l + r) / 2;
        int res = find_first(2 * id, l, mid, ql, qr, pred);
        if (res != -1) return res;
        return find_first(2 * id + 1, mid + 1, r, ql, qr, pred);
    }

    template <typename Pred>
    int find_last(int id, int l, int r, int ql, int qr, Pred&& pred) {
        if (l > qr || r < ql || !pred(tree[id])) return -1;
        if (l == r) return l;
        push(id, l, r);
        int mid = (l + r) / 2;
        int res = find_last(2 * id + 1, mid + 1, r, ql, qr, pred);
        if (res != -1) return res;
        return find_last(2 * id, l, mid, ql, qr, pred);
    }

public:
    LazySegmentTree() : n(0) {}

    explicit LazySegmentTree(int size) : n(size), tree(4 * size + 4), lazy(4 * size + 4) {}

    explicit LazySegmentTree(const std::vector<Info>& init)
        : n(static_cast<int>(init.size()) - 1), tree(4 * init.size() + 4), lazy(4 * init.size() + 4) {
        if (n >= 1) build(1, 1, n, init);
    }

    void update(int ql, int qr, const Tag& tag) {
        assert(1 <= ql && ql <= qr && qr <= n);
        update(1, 1, n, ql, qr, tag);
    }

    Info query(int ql, int qr) {
        assert(1 <= ql && ql <= qr && qr <= n);
        return query(1, 1, n, ql, qr);
    }

    /**
     * @brief O(log N) Walk on Segment Tree: Find first position in [ql, qr] satisfying pred
     */
    template <typename Pred>
    int find_first(int ql, int qr, Pred&& pred) {
        assert(1 <= ql && ql <= qr && qr <= n);
        return find_first(1, 1, n, ql, qr, std::forward<Pred>(pred));
    }

    /**
     * @brief O(log N) Walk on Segment Tree: Find last position in [ql, qr] satisfying pred
     */
    template <typename Pred>
    int find_last(int ql, int qr, Pred&& pred) {
        assert(1 <= ql && ql <= qr && qr <= n);
        return find_last(1, 1, n, ql, qr, std::forward<Pred>(pred));
    }
};

// Example Info & Tag structs for Range Add, Range Set, Range Sum, Range Min/Max
struct NodeTag {
    long long add = 0;
    long long set_val = -1; // -1 means no set update

    bool has_update() const {
        return set_val != -1 || add != 0;
    }

    void clear() {
        add = 0;
        set_val = -1;
    }

    void apply(const NodeTag& other) {
        if (other.set_val != -1) {
            set_val = other.set_val;
            add = other.add;
        } else {
            add += other.add;
        }
    }
};

struct NodeInfo {
    long long sum = 0;
    long long min_val = 0;
    long long max_val = 0;

    void apply(const NodeTag& tag, int l, int r) {
        long long len = r - l + 1;
        if (tag.set_val != -1) {
            sum = tag.set_val * len;
            min_val = tag.set_val;
            max_val = tag.set_val;
        }
        if (tag.add != 0) {
            sum += tag.add * len;
            min_val += tag.add;
            max_val += tag.add;
        }
    }

    static NodeInfo combine(const NodeInfo& left, const NodeInfo& right) {
        return {
            left.sum + right.sum,
            std::min(left.min_val, right.min_val),
            std::max(left.max_val, right.max_val)
        };
    }
};

// =========================================================================
// 3. Dynamic / Implicit Segment Tree (Up to 10^18 Coordinates)
// =========================================================================
class DynamicSegmentTree {
private:
    struct Node {
        int ls = 0;
        int rs = 0;
        long long sum = 0;
        long long lazy = 0;
    };

    long long L_bound, R_bound;
    std::vector<Node> tree;

    int new_node() {
        tree.emplace_back();
        return static_cast<int>(tree.size()) - 1;
    }

    void push(int id, long long l, long long r) {
        if (tree[id].lazy == 0) return;
        long long mid = l + (r - l) / 2;
        if (!tree[id].ls) { int nxt = new_node(); tree[id].ls = nxt; }
        if (!tree[id].rs) { int nxt = new_node(); tree[id].rs = nxt; }

        long long val = tree[id].lazy;
        tree[tree[id].ls].sum += val * (mid - l + 1);
        tree[tree[id].ls].lazy += val;
        tree[tree[id].rs].sum += val * (r - mid);
        tree[tree[id].rs].lazy += val;

        tree[id].lazy = 0;
    }

    void update(int& id, long long l, long long r, long long ql, long long qr, long long val) {
        if (!id) id = new_node();
        if (ql <= l && r <= qr) {
            tree[id].sum += val * (r - l + 1);
            tree[id].lazy += val;
            return;
        }
        push(id, l, r);
        long long mid = l + (r - l) / 2;
        if (ql <= mid) update(tree[id].ls, l, mid, ql, qr, val);
        if (qr > mid) update(tree[id].rs, mid + 1, r, ql, qr, val);
        tree[id].sum = (tree[id].ls ? tree[tree[id].ls].sum : 0) +
                       (tree[id].rs ? tree[tree[id].rs].sum : 0);
    }

    long long query(int id, long long l, long long r, long long ql, long long qr) {
        if (!id) return 0;
        if (ql <= l && r <= qr) return tree[id].sum;
        push(id, l, r);
        long long mid = l + (r - l) / 2;
        long long res = 0;
        if (ql <= mid) res += query(tree[id].ls, l, mid, ql, qr);
        if (qr > mid) res += query(tree[id].rs, mid + 1, r, ql, qr);
        return res;
    }

    int root = 0;

public:
    explicit DynamicSegmentTree(long long min_coord = 1, long long max_coord = 1000000000000000000LL)
        : L_bound(min_coord), R_bound(max_coord) {
        tree.reserve(200000);
        tree.emplace_back(); // 1-based indexing for node IDs (0 is null)
    }

    void add(long long ql, long long qr, long long val) {
        assert(L_bound <= ql && ql <= qr && qr <= R_bound);
        update(root, L_bound, R_bound, ql, qr, val);
    }

    long long query_sum(long long ql, long long qr) {
        assert(L_bound <= ql && ql <= qr && qr <= R_bound);
        return query(root, L_bound, R_bound, ql, qr);
    }
};

} // namespace cp::data_structures

// =========================================================================
// Self-Testing & Verification
// =========================================================================
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    using namespace cp::data_structures;

    // 1. Verify PointSegmentTree
    int n = 8;
    std::vector<NodeInfo> init_arr(n + 1);
    for (int i = 1; i <= n; ++i) {
        init_arr[i] = {1LL * i, 1LL * i, 1LL * i};
    }
    PointSegmentTree<NodeInfo> pst(init_arr);

    assert(pst.query(1, 8).sum == 36);
    assert(pst.query(3, 5).min_val == 3);
    assert(pst.query(3, 5).max_val == 5);

    pst.update(3, {10, 10, 10});
    assert(pst.query(3, 5).max_val == 10);
    assert(pst.query(1, 8).sum == 43);

    // 2. Verify LazySegmentTree with Add and Set
    LazySegmentTree<NodeInfo, NodeTag> lst(init_arr);

    // Range Add on [2, 4] with +5
    lst.update(2, 4, {5, -1});
    // Now arr should be: [1, 7, 8, 9, 5, 6, 7, 8]
    assert(lst.query(2, 4).sum == 7 + 8 + 9);
    assert(lst.query(1, 8).sum == 36 + 15);
    assert(lst.query(2, 4).min_val == 7);

    // Range Set on [3, 5] with 2
    lst.update(3, 5, {0, 2});
    // Now arr should be: [1, 7, 2, 2, 2, 6, 7, 8]
    assert(lst.query(3, 5).sum == 6);
    assert(lst.query(1, 8).sum == 1 + 7 + 2 + 2 + 2 + 6 + 7 + 8);

    // Test Walk on Segment Tree: Find first position >= 7 in [1, 8]
    int first_ge_7 = lst.find_first(1, 8, [](const NodeInfo& info) {
        return info.max_val >= 7;
    });
    assert(first_ge_7 == 2); // A[2] = 7

    int first_ge_8 = lst.find_first(1, 8, [](const NodeInfo& info) {
        return info.max_val >= 8;
    });
    assert(first_ge_8 == 8); // A[8] = 8

    // 3. Verify DynamicSegmentTree with large coordinates
    DynamicSegmentTree dst(1, 1000000000000000000LL);
    dst.add(100, 200, 3);
    dst.add(150, 250, 2);

    // Overlap: [100, 149] has 3; [150, 200] has 5; [201, 250] has 2
    long long exp_sum = 50LL * 3 + 51LL * 5 + 50LL * 2;
    assert(dst.query_sum(100, 250) == exp_sum);
    assert(dst.query_sum(1, 99) == 0);

    std::cout << "[SUCCESS] All Segment Tree test cases passed perfectly!\n";
    std::cout << "PST Sum: " << pst.query(1, 8).sum << "\n";
    std::cout << "LST Sum after updates: " << lst.query(1, 8).sum << "\n";
    std::cout << "First pos >= 7: " << first_ge_7 << "\n";
    std::cout << "Dynamic ST Sum [100, 250]: " << dst.query_sum(100, 250) << "\n";

    return 0;
}
